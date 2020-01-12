# CppJiebaDAT-py

## Introduct

CppJiebaDAT-py is a python API for  [CppJiebaDAT](https://git.code.oa.com/byronhe/cppjiebadat), which uses [Double Array Trie (DAT)](https://github.com/s-yata/darts-clone) with mmap  to optimize memory usage of [CppJieba](https://github.com/yanyiwu/cppjieba).

## Feature
#### Double array trie
CppJiebaDAT-py using [Double Array Triev](https://github.com/s-yata/darts-clone) with mmap to share dictionay memory (24MB mmap).

#### Default dictionary
A default dictionary with `utf8` coding（[jieba.dict.utf8]（https://git.code.oa.com/mmbizcv/cppjiebadat-py/blob/master/cppjiebadat_py/dict/jieba.dict.utf8））is package in the python libaray. Users can specify their own custom dictionary into the default dictionary to ensure a higher accuracy.


#### Delay initialization
By default, CppJiebaDAT-py don't build the prefix dictionary unless it's necessary. After initialization, CppJiebaDAT-py is not initialized again. If you want to initialize it manually, you can call:
```
import cppjiebadat_py as jieba
jieba.initialize()
```

## Usage
#### Dependencies
+ `g++ (version >= 4.1 is recommended) or clang++`;
+ `cmake (version >= 2.6 is recommended)`;

#### Compile
```
git clone http://git.code.oa.com/mmbizcv/cppjiebadat-py.git
python setup.py build
python setup.py install
```

#### Demo ([example.py](https://git.code.oa.com/mmbizcv/cppjiebadat-py/blob/master/example.py))
The most usable interfaces are same to [jieba](https://github.com/fxsjy/jieba).
```
try:
    import cppjiebadat_py as jieba
    import cppjiebadat_py.posseg as pseg
    import cppjiebadat_py.analyse as analyse
except:
    import jieba
    import jieba.posseg as pseg
    import jieba.analyse as analyse
```

## Concat
Please contact ArlenCai (arlencai@tencent.com)