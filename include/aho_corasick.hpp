#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H
struct AhoCorasickAutomaton
{
    struct Node
    {
    private:
        std::map<char, Node *> links;
        std::map<char, std::unique_ptr<Node>> child_nodes;
        std::vector<std::string> patterns;
        Node *parent{nullptr};
        Node *suff_link{nullptr};
        Node *comp_suff_link{nullptr};
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
            return !(patterns.empty());
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
            child_nodes[c] = std::make_unique<Node>(c, ptr);
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
        //therefore the automata either transitions into state by c or by suffix link
        Node *get_link(char c)
        {
            if (!has_link(c))
            {
                if (has_child(c))
                    links[c] = child_nodes[c].get();
                else if (is_root())
                    links[c] = this;
                else
                    links[c] = get_suff_link()->get_link(c);
            }
            return links[c];
        }
        //lazily define suffix link
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
        //lazily define compressed suffix link, a link to a closest terminal or to root
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
    std::unique_ptr<Node> root = nullptr;
    std::vector<std::string> dict;
    //construct automaton based on strings to search
    AhoCorasickAutomaton(const std::vector<std::string> &dict)
    {
        for (const auto &str : dict)
            this->dict.push_back(str);
    }
    //add a string char-wise, creating new nodes
    void add_string(const std::string &str)
    {
        auto cur = root.get();
        for (const auto c : str)
        {
            if (!cur->has_child(c))
                cur->add_child(c, cur);
            cur = cur->get_children()[c].get();
        }
        cur->add_patern(str);
    }
    //add all strings from dict
    void BuildAutomaton()
    {
        root = std::make_unique<Node>('\0', nullptr, true);
        for (const auto &str : this->dict)
            add_string(str);
    }

public:
    const auto process_text(const std::string &text)
    {
        std::map<std::string, std::vector<int>> result;
        auto current_state{root.get()};
        for (const auto &c : text)
        {
            current_state = current_state->get_link(c);
            auto cur_state_copy{current_state};
            while (true)
            {
                if (cur_state_copy->is_leaf())
                    for (const auto &str : cur_state_copy->get_patterns())
                        result[str].push_back(&c - text.c_str() + 1 - str.size());
                cur_state_copy = cur_state_copy->get_comp_suff_link();
                if (cur_state_copy == root.get())
                    break;
            }
        }
        return result;
    }
};
#endif