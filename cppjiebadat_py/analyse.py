#!/usr/bin/env python
# coding=utf-8
'''
@Author: ArlenCai
@Date: 2019-10-31 14:30:28
@LastEditTime: 2019-11-05 20:54:46
'''
import os
import cppjiebadat_py
get_module_res = lambda *res: os.path.normpath(os.path.join(
                        os.getcwd(), os.path.dirname(__file__), *res))
_get_abs_path = lambda path: os.path.normpath(os.path.join(os.getcwd(), path))
DEFAULT_IDF_NAME = "dict/idf.utf8";
DEFAULT_STOP_WORD_NAME = "dict/stop_words.utf8";

class TFIDF(object):
    def __init__(self, idf_path=None):
        if idf_path == None:
            self.idf_path = get_module_res(DEFAULT_IDF_NAME)
        else:
            self.idf_path = _get_abs_path(idf_path)
        self.stop_word_path = get_module_res(DEFAULT_STOP_WORD_NAME)
        #self.initialized = False

    def initialize(self):
        cppjiebadat_py.check_initialized()
        cppjiebadat_py._init_keyowrd_extractor(self.idf_path, self.stop_word_path)
        #self.initialized = True

    def set_stop_words(self, stop_words_path):
        abs_path = _get_abs_path(stop_words_path)
        if not os.path.isfile(abs_path):
            raise Exception("cppjieba: file does not exist: " + abs_path)
        self.stop_word_path = abs_path
        #iself.initialized = False

    def set_idf_path(self, idf_path):
        abs_path = _get_abs_path(idf_path)
        if not os.path.isfile(abs_path):
            raise Exception("cppjieba: file does not exist: " + abs_path)
        self.idf_path = abs_path
        #self.initialized = False

    def extract_tags(self, sentence, topK=20, withWeight=False):
        #if not self.initialized or :
        self.initialize()
        if not withWeight:
            return cppjiebadat_py._extract_tags_no_weight(sentence, topK)
        else:
            return cppjiebadat_py._extract_tags_with_weight(sentence, topK)

class TextRank(object):
    def __init__(self):
        self.stop_word_path = get_module_res(DEFAULT_STOP_WORD_NAME)
        #self.initialized = False

    def initialize(self):
        cppjiebadat_py.check_initialized()
        cppjiebadat_py._init_textrank_extractor(self.stop_word_path)
        #self.initialized = True

    def set_stop_words(self, stop_words_path):
        abs_path = _get_abs_path(stop_words_path)
        if not os.path.isfile(abs_path):
            raise Exception("cppjieba: file does not exist: " + abs_path)
        self.stop_word_path = abs_path
        #self.initialized = False

    def textrank(self, sentence, topK=20, withWeight=False):
        #if not self.initialized:
        self.initialize()
        if not withWeight:
            return cppjiebadat_py._textrank_no_weight(sentence, topK)
        else:
            return cppjiebadat_py._textrank_with_weight(sentence, topK)

default_tfidf = TFIDF()
default_textrank = TextRank()

extract_tags = tfidf = default_tfidf.extract_tags
set_idf_path = default_tfidf.set_idf_path
textrank = default_textrank.textrank
