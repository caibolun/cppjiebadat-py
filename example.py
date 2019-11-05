#!/usr/bin/env python
# coding=utf-8
'''
@Author: ArlenCai
@Date: 2019-10-31 14:30:28
@LastEditTime: 2019-11-05 20:58:33
'''
from cppjiebadat_py import Tokenizer, cut, tokenize, cut_for_search, lcut, lcut_for_search, initialize, load_userdict
from cppjiebadat_py import posseg as pseg
import datetime
from cppjiebadat_py import analyse
from cppjiebadat_py.analyse import TextRank,TFIDF

def main():

    jieba_instance = Tokenizer()
    
    seg_list = jieba_instance.cut("我来到北京清华大学", cut_all = True)
    print(type(seg_list))
    print("Full Mode: " + "/ ".join(seg_list))  # 全模式
    
    seg_list = jieba_instance.cut("他来到了网易杭研大厦")  # 默认是精确模式
    print(", ".join(seg_list))
    
    seg_list = jieba_instance.cut_for_search(
        "小明硕士毕业于中国科学院计算所，后在日本京都大学深造")  # 搜索引擎模式
    print(", ".join(seg_list))
    
    t1 = datetime.datetime.now()
    initialize()
    t2 = datetime.datetime.now()
    print("initialize costs:%s" % (t2 - t1))
    
    print(lcut("我来到北京清华大学"))
    print(list(cut("我来到北京清华大学")))
    print(cut("我来到北京清华大学",cut_all=True))
    print(lcut_for_search("我来到北京清华大学"))
    print(list(cut_for_search("我来到北京清华大学")))
    
    print(pseg.lcut("我来到北京清华大学"))
    print(list(pseg.cut("我来到北京清华大学")))
    
    s = "此外，公司拟对全资子公司吉林欧亚置业有限公司增资4.3亿元，增资后，吉林欧亚置业注册资本由7000万元增加到5亿元。吉林欧亚置业主要经营范围为房地产开发及百货零售等业务。目前在建吉林欧亚城市商业综合体项目。2013年，实现营业收入0万元，实现净利润-139.13万元。"
    r = analyse.extract_tags(s)
    print(r)

    r = analyse.textrank(s, withWeight=True)
    print(r)

    result = jieba_instance.tokenize('永和服装饰品有限公司')
    for tk in result:
        print("word %s\t\t start: %d \t\t end:%d" % (tk[0],tk[1],tk[2]))

    print(tokenize('永和服装饰品有限公司',mode="search"))

if __name__ == '__main__':
    main()
