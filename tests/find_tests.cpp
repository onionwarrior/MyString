#include "testing_utilites.hpp"
#include <algorithm>
void do_find_tests()
{
    MyString test_string("lorem ipsum");
    assert(test_string.find("lorem ipsum")==0);
    assert(test_string.find("ipsum lorem")==-1);
    assert(test_string.find("i")==6);
    assert(test_string.find("m")==4);
    assert(test_string.find("m",5)==10);
    assert(test_string.find("lorem")==0);
    assert(test_string.find("rem")==2);
    assert(test_string.find("sum")==8);
    std::vector<std::string> test_dict{"gun","egg","a q","quick brown","uick brown","qiu","laz","dogo","a quick brown fox","ick brown fo","ck brown fo"};
    test_string="a quick brown fox jumps over a lazy dog";
    auto res=test_string.find(test_dict);
    std::vector<std::string> strs;
    for(const auto & p: res){
        strs.push_back(p.first);
    }
    std::sort(strs.begin(),strs.end());
    std::vector<std::string> ans{"a q","quick brown","uick brown","laz","a quick brown fox","ick brown fo","ck brown fo"};
    std::sort(ans.begin(),ans.end());
    assert(ans==strs);
}