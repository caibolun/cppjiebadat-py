/*
 * @Author: ArlenCai
 * @Date: 2019-10-30 13:06:27
 * @LastEditTime: 2019-10-30 20:28:41
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cppjieba/Jieba.hpp"
#include <iostream>

using namespace std;
namespace py = pybind11;

const string DICT_PATH = "cppjiebadat/dict/jieba.dict.utf8";
const string HMM_PATH = "cppjiebadat/dict/hmm_model.utf8";
const string IDF_PATH = "cppjiebadat/dict/idf.utf8";
const string STOP_WORD_PATH = "cppjiebadat/dict/stop_words.utf8";

struct JiebaCpp
{
    cppjieba::Jieba jieba;

  public:
    JiebaCpp(const string &USER_DICT_PATH) : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH){};
    JiebaCpp(const string &DICT_PATH, const string &USER_DICT_PATH) : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH){};
    JiebaCpp(const string &DICT_PATH, const string &HMM_PATH, const string &USER_DICT_PATH, const string &IDF_PATH, const string &STOP_WORD_PATH) : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH){};

    vector<string> cut(string &text)
    {
        vector<string> words;
        jieba.Cut(text, words, true);
        return words;
    };

    vector<string> cutForSearch(string &text)
    {
        vector<string> words;
        jieba.CutForSearch(text, words, true);
        return words;
    };

    vector<pair<string, string>> tag(const string &sentence)
    {
        vector<pair<string, string> > words;
        jieba.Tag(sentence, words);
        return words;
    };


    /*
    void InsertUserWord(std::string &word)
    {
        jieba.InsertUserWord(word);
    };
    */
};

PYBIND11_MODULE(cppjiebadat_py, m)
{
    m.doc() = "python extension for cppjiebadat"; // optional module docstring

    py::class_<JiebaCpp>(m, "jieba")
        .def(py::init<const string &>())
        .def(py::init<const string &, const string &>())
        .def(py::init<const string &, const string &, const string &, const string &, const string &>())
        .def("cut", &JiebaCpp::cut)
        .def("cut_for_search", &JiebaCpp::cutForSearch)
        .def("tag", &JiebaCpp::tag);
}