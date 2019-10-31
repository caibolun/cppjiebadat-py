#!/usr/bin/env python
# coding=utf-8
'''
@Author: ArlenCai
@Date: 2019-10-30 13:06:27
@LastEditTime: 2019-10-30 20:32:44
'''
from cppjiebadat_py import jieba 

def main():

    HMM_PATH = "cppjiebadat/dict/hmm_model.utf8"
    IDF_PATH = "cppjiebadat/dict/idf.utf8"
    STOP_WORD_PATH = "cppjiebadat/dict/stop_words.utf8"
    jieba_instance = jieba("cppjiebadat/dict/jieba.dict.utf8", HMM_PATH, "cppjiebadat/dict/user.dict.utf8", IDF_PATH, STOP_WORD_PATH)
    seg_list = jieba_instance.cut("我来到北京清华大学")
    print("Full Mode: " + "/ ".join(seg_list))  # 全模式


    seg_list = jieba_instance.cut("他来到了网易杭研大厦")  # 默认是精确模式
    print(", ".join(seg_list))

    seg_list = jieba_instance.cut_for_search(
        "小明硕士毕业于中国科学院计算所，后在日本京都大学深造")  # 搜索引擎模式
    print(", ".join(seg_list))
    
    seg_list = jieba_instance.tag("我来到北京清华大学")
    print(seg_list)


if __name__ == '__main__':
    main()
