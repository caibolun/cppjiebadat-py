#include "cppjieba/DictTrie.hpp"
#include "cppjieba/MPSegment.hpp"
#include "gtest/gtest.h"

using namespace cppjieba;
static const char* const DICT_FILE = "../test/testdata/extra_dict/jieba.dict.small.utf8";

TEST(TrieTest, Empty) {
  vector<DatElement> elements;
  DatTrie trie;
  trie.InitBuildDat(elements, "elements.dat", "d41d8cd98f00b204e9800998ecf8427e");
}

TEST(TrieTest, Construct) {
  vector<DatElement> elements;
  DatElement e;
  e.word = "你";
  DatTrie trie;
  trie.InitBuildDat(elements, "elements.dat","a620945bae93bcd5b8b0a75f8a327ce4");
}

TEST(DictTrieTest, NewAndDelete) {
  DictTrie * trie;
  trie = new DictTrie(DICT_FILE);
  delete trie;
}

TEST(DictTrieTest, Test1) {
  string s1, s2;
  DictTrie trie(DICT_FILE);
  ASSERT_LT(trie.GetMinWeight() + 15.6479, 0.001);
  string word("来到");
  cppjieba::RuneStrArray uni;
  ASSERT_TRUE(DecodeRunesInString(word, uni));
  //DictUnit nodeInfo;
  //nodeInfo.word = uni;
  //nodeInfo.tag = "v";
  //nodeInfo.weight = -8.87033;
  //s1 << nodeInfo;
  //s2 << (*trie.Find(uni.begin(), uni.end()));
  const auto* du = trie.Find(word);
  ASSERT_TRUE(du != NULL);
  //ASSERT_EQ(2u, du->word.size());
  //ASSERT_EQ(26469u, du->word[0]);
  //ASSERT_EQ(21040u, du->word[1]);
  ASSERT_EQ("v", du->GetTag());
  ASSERT_NEAR(-8.870, du->weight, 0.001);

  //EXPECT_EQ("[\"26469\", \"21040\"] v -8.870", s2);
  word = "清华大学";
  LocalVector<pair<size_t, const DatMemElem *> > res;
  const char * words[] = {"清", "清华", "清华大学"};
  for (size_t i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
      ASSERT_TRUE(DecodeRunesInString(words[i], uni));
    res.push_back(make_pair(uni.size() , trie.Find(words[i])));
    //resMap[uni.size() - 1] = trie.Find(uni.begin(), uni.end());
  }
  vector<pair<size_t, const DatMemElem*> > vec;
  vector<struct DatDag> dags;
  ASSERT_TRUE(DecodeRunesInString(word, uni));
  trie.Find(uni.begin(), uni.end(), dags);
  ASSERT_EQ(dags.size(), uni.size());
  ASSERT_NE(dags.size(), 0u);
  s1 << res;
  s2 << dags[0].nexts;
  ASSERT_EQ(s1, s2);
}

TEST(DictTrieTest, UserDict) {
  DictTrie trie(DICT_FILE, "../test/testdata/userdict.utf8");
  string word = "云计算";
  const auto * unit = trie.Find(word);
  ASSERT_NE(unit, nullptr);
  ASSERT_NEAR(unit->weight, -14.100, 0.001);

  word = "蓝翔";
  unit = trie.Find(word);
  ASSERT_TRUE(unit != NULL);
  ASSERT_EQ(unit->GetTag(), "nz");
  ASSERT_NEAR(unit->weight, -14.100, 0.001);

  word = "区块链";
  unit = trie.Find(word);
  ASSERT_TRUE(unit != NULL);
  ASSERT_EQ(unit->GetTag(), "nz");
  ASSERT_NEAR(unit->weight, -15.6478, 0.001);
}

TEST(DictTrieTest, UserDictWithMaxWeight) {
  DictTrie trie(DICT_FILE, "../test/testdata/userdict.utf8", "", DictTrie::WordWeightMax);
  string word = "云计算";
  const auto * unit = trie.Find(word);
  ASSERT_TRUE(unit);
  ASSERT_NEAR(unit->weight, -2.975, 0.001);
}

TEST(DictTrieTest, Dag) {
  DictTrie trie(DICT_FILE, "../test/testdata/userdict.utf8");

  {
    string word = "清华大学";
    cppjieba::RuneStrArray unicode;
    ASSERT_TRUE(DecodeRunesInString(word, unicode));
    vector<struct DatDag> res;
    trie.Find(unicode.begin(), unicode.end(), res);

    size_t nexts_sizes[] = {3, 2, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  {
    string word = "北京邮电大学";
    cppjieba::RuneStrArray unicode;
    ASSERT_TRUE(DecodeRunesInString(word, unicode));
    vector<struct DatDag> res;
    trie.Find(unicode.begin(), unicode.end(), res);

    size_t nexts_sizes[] = {3, 1, 2, 2, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  {
    string word = "长江大桥";
    cppjieba::RuneStrArray unicode;
    ASSERT_TRUE(DecodeRunesInString(word, unicode));
    vector<struct DatDag> res;
    trie.Find(unicode.begin(), unicode.end(), res);

    size_t nexts_sizes[] = {3, 1, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  {
    string word = "长江大桥";
    cppjieba::RuneStrArray unicode;
    ASSERT_TRUE(DecodeRunesInString(word, unicode));
    vector<struct DatDag> res;
    trie.Find(unicode.begin(), unicode.end(), res, 3);

    size_t nexts_sizes[] = {2, 1, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }

  {
    string word = "长江大桥";
    cppjieba::RuneStrArray unicode;
    ASSERT_TRUE(DecodeRunesInString(word, unicode));
    vector<struct DatDag> res;
    trie.Find(unicode.begin(), unicode.end(), res, 4);

    size_t nexts_sizes[] = {3, 1, 2, 1};
    ASSERT_EQ(res.size(), sizeof(nexts_sizes)/sizeof(nexts_sizes[0]));
    for (size_t i = 0; i < res.size(); i++) {
      ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
    }
  }
}

