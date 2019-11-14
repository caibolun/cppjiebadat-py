#ifndef SITE_PACKAGE_PATH
#define SITE_PACKAGE_PATH STR_VALUE(SITE_PACKAGE_PATH)
#endif
#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/stl.h>
#include "cppjieba/Jieba.hpp"
#include "cppjieba/TextRankExtractor.hpp"
#include <iostream>

using namespace std;
namespace py = pybind11;

using Word = cppjieba::Word;

using WordVector = vector<string>;

using WordsTaged = vector<pair<string, string>>;

struct Tokenizer
{
    cppjieba::Jieba *jieba;
    cppjieba::KeywordExtractor *keywordExtractor;
    cppjieba::TextRankExtractor *textRankExtractor;
    bool keyword_needinit;
    bool textRank_needinit;
  public:
    Tokenizer()
    {
        jieba = NULL;
        keywordExtractor = NULL;
        textRankExtractor = NULL;
        keyword_needinit = true;
        textRank_needinit = true;
    }
    ~Tokenizer()
    {
        release();
    }
    void release()
    {
        //cout<<"release... ..."<<endl;
        if(jieba)
        {
            delete jieba;
            jieba = NULL;
        }
        if(keywordExtractor)
        {
            delete keywordExtractor;
            keywordExtractor = NULL;
        }
        if(textRankExtractor)
        {
            delete textRankExtractor;
            textRankExtractor = NULL;
        }
    }
    void initialize(const string &main_dict, const string &hmm_path, const string &user_dict, const string &idf_path,const string &stop_word_path)
    {
        //cout<<"init... ..."<<endl;
        if(jieba) delete jieba;
        jieba = new cppjieba::Jieba(main_dict, hmm_path, user_dict, idf_path, stop_word_path);
        keyword_needinit = true;
        textRank_needinit = true;
    };

    vector<tuple<string, uint32_t, uint32_t>> tokenize(const string &sentence, const string &mode = "default", bool HMM = true)
    {
        vector<tuple<string, uint32_t, uint32_t>> result;
        vector<Word> words;
        if (mode.compare("default") == 0)
        {
            jieba->Cut(sentence, words, HMM);
        }
        else
        {
            jieba->CutForSearch(sentence, words, HMM);
        }

        vector<Word>::const_iterator it;
        it = words.begin();
        while (it != words.end())
        {
            result.push_back(make_tuple(it->word, it->unicode_offset, it->unicode_offset + it->unicode_length));
            ++it;
        }
        return result;
    };

    WordVector lcut_internal(const string &sentence, bool cut_all = false, bool HMM = true)
    {
        WordVector words;
        if (!cut_all)
        {
            jieba->Cut(sentence, words, HMM);
        }
        else
        {
            jieba->CutAll(sentence, words);
        }
        return words;
    };


    WordVector lcut_for_search_internal(const string &sentence, bool HMM = true)
    {
        WordVector words;
        jieba->CutForSearch(sentence, words, HMM);
        return words;
    };

    WordsTaged tag(const string &sentence)
    {
        WordsTaged words;
        jieba->Tag(sentence, words);
        return words;
    };

    bool find(const string &word)
    {
        return jieba->Find(word);
    };

    string lookup_tag(const string &word) const
    {
        return jieba->LookupTag(word);
    };

    // KeywordExtractor
    void init_keyowrd_extractor(const string &idfPath,
                              const string &stopWordPath)
    {
        if (keyword_needinit)
        {
            //cout<<"init_keyowrd_extractor ... ..."<<endl;
            if(keywordExtractor) delete keywordExtractor;
            keywordExtractor = new cppjieba::KeywordExtractor(jieba->GetDictTrie(), jieba->GetHMMModel(), idfPath, stopWordPath);
            keyword_needinit = false;
        }
    };

    vector<string> extract_tags_no_weight(const string &sentence, size_t topK = 20)
    {
        vector<string> keywords;
        keywordExtractor->Extract(sentence, keywords, topK);
        return keywords;
    };
    vector<pair<string, double>> extract_tags_with_weight(const string &sentence, size_t topK = 20)
    {
        vector<pair<string, double>> keywords;
        keywordExtractor->Extract(sentence, keywords, topK);
        return keywords;
    };
    
    //TextRankExtractor
    void init_textrank_extractor(const string &stopWordPath)
    {
        if (textRank_needinit)
        {
            //cout<<"init_textrank_extractor ... ..."<<endl;
            if(textRankExtractor) delete textRankExtractor;
            textRankExtractor = new cppjieba::TextRankExtractor(jieba->GetDictTrie(), jieba->GetHMMModel(), stopWordPath);
            textRank_needinit = false;
        }
    };
    vector<string> textrank_no_weight(const string &sentence, size_t topK = 20)
    {
        vector<string> keywords;
        textRankExtractor->Extract(sentence, keywords, topK);
        return keywords;
    };
    vector<pair<string, double>> textrank_with_weight(const string &sentence, size_t topK = 20)
    {
        vector<pair<string, double>> keywords;
        textRankExtractor->Extract(sentence, keywords, topK);
        return keywords;
    };
};

PYBIND11_MODULE(libcppjiebadat, m)
{
    m.doc() = "python extension for cppjiebadat"; // optional module docstring

    py::class_<Tokenizer>(m, "cppjiebadat")
        .def(py::init<>())
        .def("_release", &Tokenizer::release)
        .def("_initialize", &Tokenizer::initialize)
        .def("_lcut_internal", &Tokenizer::lcut_internal, py::arg("sentence"), py::arg("cut_all") = false, py::arg("HMM") = true)
        .def("_lcut_for_search_internal", &Tokenizer::lcut_for_search_internal, py::arg("sentence"), py::arg("HMM") = true)
        .def("_tag", &Tokenizer::tag, py::arg("sentence"))
        .def("_tokenize", &Tokenizer::tokenize, py::arg("sentence"), py::arg("mode") = "default", py::arg("HMM") = true)
        .def("_find", &Tokenizer::find)
        .def("_lookup_tag", &Tokenizer::lookup_tag)
        .def("_init_keyowrd_extractor", &Tokenizer::init_keyowrd_extractor)
        .def("_extract_tags_no_weight", &Tokenizer::extract_tags_no_weight)
        .def("_extract_tags_with_weight", &Tokenizer::extract_tags_with_weight)
        .def("_init_textrank_extractor", &Tokenizer::init_textrank_extractor)
        .def("_textrank_no_weight", &Tokenizer::textrank_no_weight)
        .def("_textrank_with_weight", &Tokenizer::textrank_with_weight);
}
