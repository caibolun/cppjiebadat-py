#!/usr/bin/env python
# coding=utf-8
'''
@Author: ArlenCai
@Date: 2019-10-31 14:30:28
@LastEditTime: 2019-11-05 22:17:54
'''
import os
get_module_res = lambda *res: os.path.normpath(os.path.join(
                        os.getcwd(), os.path.dirname(__file__), *res))
                        
DEFAULT_DICT_NAME = "dict/jieba.dict.utf8";
DEFAULT_HMM_NAME = "dict/hmm_model.utf8";
DEFAULT_IDF_NAME = "dict/idf.utf8";
DEFAULT_STOP_WORD_NAME = "dict/stop_words.utf8";
from libcppjiebadat import cppjiebadat

_get_abs_path = lambda path: os.path.normpath(os.path.join(os.getcwd(), path))

DEFAULT_DICT = None

class Tokenizer(cppjiebadat):
    def __init__(self, dictionary=DEFAULT_DICT):
        if dictionary == DEFAULT_DICT:
            self.dictionary = get_module_res(DEFAULT_DICT_NAME)
        else:
            self.dictionary = _get_abs_path(dictionary)
        self.userdict = ""
        self.hmm_path = get_module_res(DEFAULT_HMM_NAME)
        self.idf_path = get_module_res(DEFAULT_IDF_NAME)
        self.stop_word_path = get_module_res(DEFAULT_STOP_WORD_NAME)
        self.initialized = False

    def initialize(self, dictionary=None):
        if dictionary:
            abs_path = _get_abs_path(dictionary)
            if self.dictionary == abs_path and self.initialized:
                return
            else:
                self.dictionary = abs_path
                self.initialized = False
        else:
            abs_path = self.dictionary
        self._initialize(self.dictionary, self.hmm_path, self.userdict, self.idf_path, self.stop_word_path)
        self.initialized = True
    
    def check_initialized(self):
        if not self.initialized:
            self.initialize()

    def lcut(self, sentence, cut_all=False, HMM=True):
        self.check_initialized()
        return self._lcut_internal(sentence, cut_all, HMM)
    
    def lcut_for_search(self, sentence, HMM=True):
        self.check_initialized()
        return self._lcut_for_search_internal(sentence, HMM)
    
    def cut(self, *args, **kwargs):
        return iter(self.lcut(*args, **kwargs))

    def cut_for_search(self, *args, **kwargs):
        return iter(self.lcut_for_search(*args, **kwargs))

    def tokenize(self, sentence, mode="default", HMM=True):
        self.check_initialized()
        return self._tokenize(sentence, mode, HMM)

    def set_dictionary(self, dictionary_path):
        abs_path = _get_abs_path(dictionary_path)
        if not os.path.isfile(abs_path):
            raise Exception("cppjieba: file does not exist: " + abs_path)
        self.dictionary = abs_path
        self.initialized = False
    
    def get_dict_file(self):
        if self.dictionary == DEFAULT_DICT:
            return get_module_res(DEFAULT_DICT_NAME)
        else:
            return self.dictionary
    
    def set_userdict(self, userdict_path):
        abs_path = _get_abs_path(userdict_path)
        if not os.path.isfile(abs_path):
            raise Exception("cppjieba: file does not exist: " + abs_path)
        self.userdict = abs_path
        self.initialized = False

    def load_userdict(self, userdict_path):
        if isinstance(userdict_path, str):
            self.set_userdict(userdict_path)
        else:
            raise Exception("cppjieba: the input parameter only support filepath now.")
    
    def tag(self, sentence):
        self.check_initialized()
        return self._tag(sentence)
    
    def _extract_tags(self, sentence, topK=20, withWeight=False):
        self.check_initialized()
        if not withWeight:
            return self._extract_tags_no_weight(sentence, topK)
        else:
            return self._extract_tags_with_weight(sentence, topK)
    
    def _textrank(self,sentence, topK=20, withWeight=False):
        self.check_initialized()
        if not withWeight:
            return self._textrank_no_weight(sentence, topK)
        else:
            return self._textrank_with_weight(sentence, topK)

    

# default Tokenizer instance

dt = Tokenizer()

# global functions

# get_FREQ = lambda k, d=None: dt.FREQ.get(k, d)
# calc = dt.calc
cut = dt.cut
lcut = dt.lcut
cut_for_search = dt.cut_for_search
lcut_for_search = dt.lcut_for_search
get_dict_file = dt.get_dict_file
initialize = dt.initialize
check_initialized = dt.check_initialized
load_userdict = dt.load_userdict
set_dictionary = dt.set_dictionary
# suggest_freq = dt.suggest_freq
tokenize = dt.tokenize
tag = dt.tag
# user_word_tag_tab = dt.user_word_tag_tab

_init_keyowrd_extractor = dt._init_keyowrd_extractor
_extract_tags_no_weight = dt._extract_tags_no_weight
_extract_tags_with_weight = dt._extract_tags_with_weight
_init_textrank_extractor = dt._init_textrank_extractor
_textrank_no_weight = dt._textrank_no_weight
_textrank_with_weight = dt._textrank_with_weight