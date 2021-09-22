#pragma once
#include <vector>
#include <map>
struct AhoCorasickAutomaton
{
    struct Node
    {
    private:
        std::map<char, Node *> child_nodes;
        std::map<char, Node *> links;
        std::vector<std::string> patterns;
        Node *parent = nullptr;
        Node *suff_link = nullptr;
        Node *comp_suff_link = nullptr;
        bool is_root_;
        char to_parent_;

    public:
        Node(char c, Node *parent, bool is_root = false) : parent{parent},
                                                           is_root_{is_root},
                                                           to_parent_{c}
        {
        }
        bool is_leaf() const
        {
            return child_nodes.empty();
        }
        bool has_suff_link() const
        {
            return suff_link != nullptr;
        }
        bool is_root() const
        {
            return is_root_;
        }
        bool has_child(char c) const
        {
            return child_nodes.find(c) != child_nodes.end();
        }
        bool has_link(char c) const
        {
            return links.find(c) != links.end();
        }
        void add_child(char c, Node *ptr)
        {
            child_nodes[c] = ptr;
        }
        void add_patern(const std::string &pattern)
        {
            patterns.push_back(pattern);
        }
        auto &get_patterns()
        {
            return patterns;
        }
        auto &get_children()
        {
            return child_nodes;
        }
        //All automaton links are stored in a map for each state, such that (state[i]-(c)>state[j])
        Node *get_link(char c)
        {
            if (!has_link(c))
            {
                if (has_child(c))
                    links[c] = child_nodes[c];
                else if (is_root())
                    links[c] = this;
                else
                    links[c] = get_suff_link()->get_link(c);
            }
            return links[c];
        }
        Node *get_suff_link()
        {
            if (has_suff_link())
                return suff_link;
            if (is_root() || parent->is_root())
            {
                if (is_root())
                    suff_link = this;
                else
                    suff_link = parent;
            }
            else
                suff_link = (parent->get_suff_link())->get_link(to_parent_);
            return suff_link;
        }
        Node *get_comp_suff_link()
        {
            if (comp_suff_link == nullptr)
            {
                if (get_suff_link()->is_leaf())
                    comp_suff_link = get_suff_link();
                else if (is_root())
                    comp_suff_link = this;
                else
                    comp_suff_link = get_suff_link()->get_comp_suff_link();
            }
            return comp_suff_link;
        }
    };
    Node *root = nullptr;
    std::vector<std::string> dict;
    //construct automaton based on strings to search
    AhoCorasickAutomaton(std::vector<const char *> dict)
    {
        for (const auto &str : dict)
            this->dict.emplace_back(std::move(std::string(str)));
    }
    //add a string char-wise, creating new nodes
    void add_string(const std::string &str)
    {
        Node *cur = root;
        for (const auto c : str)
        {
            if (!cur->has_child(c))
            {
                cur->add_child(c, new Node(c, cur));
            }
            cur = cur->get_children()[c];
        }
        cur->add_patern(str);
    }
    //add all strings from dict
    void BuildAutomaton()
    {
        root = new Node('\0', nullptr, true);
        for (const auto &str : this->dict)
            add_string(str);
    }

public:
    const std::map < std::string, std::vector < int> > process_text(const std::string &text)
    {
        std::map < std::string, std::vector <int> > result;
        Node *current_state = root;
        for (const auto &c : text)
        {
            current_state = current_state->get_link(c);
            if (!current_state->get_patterns().empty())
            {
                for (const auto &str : current_state->get_patterns())
                    result[str].push_back(&c - text.c_str()+1-str.size());
            }
            if (current_state->is_leaf())
            {
                current_state = current_state->get_comp_suff_link();
            }
        }
        return result;
    }
};