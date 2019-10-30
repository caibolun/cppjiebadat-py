/*
 * @Author: ArlenCai
 * @Date: 2019-10-30 13:06:27
 * @LastEditTime: 2019-10-30 13:17:25
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cppjieba/Jieba.hpp"
#include <iostream>

namespace py = pybind11;

const std::string DICT_PATH = "cppjiebadat/dict/jieba.dict.utf8";
const std::string HMM_PATH = "cppjiebadat/dict/hmm_model.utf8";
const std::string IDF_PATH = "cppjiebadat/dict/idf.utf8";
const std::string STOP_WORD_PATH = "cppjiebadat/dict/stop_words.utf8";

struct JiebaCpp
{
    cppjieba::Jieba jieba;

  public:
    JiebaCpp(const std::string &USER_DICT_PATH) : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH){};

    std::vector<std::string> cut(std::string &text)
    {
        std::vector<std::string> words;
        jieba.Cut(text, words, true);
        return words;
    };

    std::vector<std::string> cutForSearch(std::string &text)
    {
        std::vector<std::string> words;
        jieba.CutForSearch(text, words, true);
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
        .def(py::init<const std::string &>())
        .def("cut", &JiebaCpp::cut)
        .def("cut_for_search", &JiebaCpp::cutForSearch);
        //.def("add_word", &JiebaCpp::InsertUserWord);
}