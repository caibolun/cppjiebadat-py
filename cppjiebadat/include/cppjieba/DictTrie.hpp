#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdint.h>
#include <cmath>
#include <limits>
#include "limonp/StringUtil.hpp"
#include "limonp/Logging.hpp"
#include "Unicode.hpp"
#include "DatTrie.hpp"

namespace cppjieba {

using namespace limonp;

const double MIN_DOUBLE = -3.14e+100;
const double MAX_DOUBLE = 3.14e+100;
const size_t DICT_COLUMN_NUM = 3;
const char* const UNKNOWN_TAG = "";

class DictTrie {
public:
    enum UserWordWeightOption {
        WordWeightMin,
        WordWeightMedian,
        WordWeightMax,
    }; // enum UserWordWeightOption

    DictTrie(const string& dict_path, const string& user_dict_paths = "", const string & dat_cache_path = "",
             UserWordWeightOption user_word_weight_opt = WordWeightMax) {
        Init(dict_path, user_dict_paths, dat_cache_path, user_word_weight_opt);
    }

    ~DictTrie() {}

    const DatMemElem* Find(const string & word) const {
        return dat_.Find(word);
    }

    void Find(RuneStrArray::const_iterator begin,
              RuneStrArray::const_iterator end,
              vector<struct DatDag>&res,
              size_t max_word_len = MAX_WORD_LENGTH) const {
        dat_.Find(begin, end, res, max_word_len);
    }

    bool IsUserDictSingleChineseWord(const Rune& word) const {
        return IsIn(user_dict_single_chinese_word_, word);
    }

    double GetMinWeight() const {
        return dat_.GetMinWeight();
    }

    void InserUserDictNode(const string& line, bool saveNodeInfo = true) {
        vector<string> buf;
        DatElement node_info;
        Split(line, buf, " ");

        if (buf.size() == 0) {
            return;
        }

        node_info.word = buf[0];
        node_info.weight = user_word_default_weight_;
        node_info.tag = UNKNOWN_TAG;

        if (buf.size() == 2) {
            node_info.tag = buf[1];
        } else if (buf.size() == 3) {
            if (freq_sum_ > 0.0) {
                const int freq = atoi(buf[1].c_str());
                node_info.weight = log(1.0 * freq / freq_sum_);
                node_info.tag = buf[2];
            }
        }

        if (saveNodeInfo) {
            static_node_infos_.push_back(node_info);
        }

        if (Utf8CharNum(node_info.word) == 1) {
            RuneArray word;

            if (DecodeRunesInString(node_info.word, word)) {
                user_dict_single_chinese_word_.insert(word[0]);
            } else {
                XLOG(ERROR) << "Decode " << node_info.word << " failed.";
            }
        }
    }

    void LoadUserDict(const string& filePaths, bool saveNodeInfo = true) {
        vector<string> files = limonp::Split(filePaths, "|;");

        for (size_t i = 0; i < files.size(); i++) {
            ifstream ifs(files[i].c_str());
            XCHECK(ifs.is_open()) << "open " << files[i] << " failed";
            string line;

            for (; getline(ifs, line);) {
                if (line.size() == 0) {
                    continue;
                }

                InserUserDictNode(line, saveNodeInfo);
            }
        }
    }


private:
    void Init(const string& dict_path, const string& user_dict_paths, string dat_cache_path,
              UserWordWeightOption user_word_weight_opt) {
        const auto dict_list = dict_path + "|" + user_dict_paths;
        const string md5 = CalcFileListMD5(dict_list);

        if (dat_cache_path.empty()) {
            string cache_id;
            const auto ret = limonp::md5String(dict_list.c_str(), cache_id);
            assert(ret);
            dat_cache_path = dict_path + "." + cache_id + "." + to_string(user_word_weight_opt) +  ".dat_cache";
        }

        if (dat_.InitAttachDat(dat_cache_path, md5)) {
            LoadUserDict(user_dict_paths, true); // for load user_dict_single_chinese_word_;
            return;
        }

        LoadDefaultDict(dict_path);
        freq_sum_ = CalcFreqSum(static_node_infos_);
        CalculateWeight(static_node_infos_, freq_sum_);
        double min_weight = 0;
        SetStaticWordWeights(user_word_weight_opt, min_weight);
        dat_.SetMinWeight(min_weight);

        LoadUserDict(user_dict_paths);
        const auto build_ret = dat_.InitBuildDat(static_node_infos_, dat_cache_path, md5);
        assert(build_ret);
        vector<DatElement>().swap(static_node_infos_);
    }

    void LoadDefaultDict(const string& filePath) {
        ifstream ifs(filePath.c_str());
        XCHECK(ifs.is_open()) << "open " << filePath << " failed.";
        string line;
        vector<string> buf;

        for (; getline(ifs, line);) {
            Split(line, buf, " ");
            XCHECK(buf.size() == DICT_COLUMN_NUM) << "split result illegal, line:" << line;
            DatElement node_info;
            node_info.word = buf[0];
            node_info.weight = atof(buf[1].c_str());
            node_info.tag = buf[2];
            static_node_infos_.push_back(node_info);
        }
    }

    static bool WeightCompare(const DatElement& lhs, const DatElement& rhs) {
        return lhs.weight < rhs.weight;
    }

    void SetStaticWordWeights(UserWordWeightOption option, double & min_weight) {
        XCHECK(!static_node_infos_.empty());
        vector<DatElement> x = static_node_infos_;
        sort(x.begin(), x.end(), WeightCompare);
        min_weight = x[0].weight;
        const double max_weight_ = x[x.size() - 1].weight;
        const double median_weight_ = x[x.size() / 2].weight;

        switch (option) {
            case WordWeightMin:
                user_word_default_weight_ = min_weight;
                break;

            case WordWeightMedian:
                user_word_default_weight_ = median_weight_;
                break;

            default:
                user_word_default_weight_ = max_weight_;
                break;
        }
    }

    double CalcFreqSum(const vector<DatElement>& node_infos) const {
        double sum = 0.0;

        for (size_t i = 0; i < node_infos.size(); i++) {
            sum += node_infos[i].weight;
        }

        return sum;
    }

    void CalculateWeight(vector<DatElement>& node_infos, double sum) const {
        assert(sum > 0.0);

        for (size_t i = 0; i < node_infos.size(); i++) {
            DatElement& node_info = node_infos[i];
            assert(node_info.weight > 0.0);
            node_info.weight = log(double(node_info.weight) / sum);
        }
    }

private:
    vector<DatElement> static_node_infos_;
    DatTrie dat_;

    double freq_sum_;
    double user_word_default_weight_;
    unordered_set<Rune> user_dict_single_chinese_word_;
};
}

