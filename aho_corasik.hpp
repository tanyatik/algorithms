#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <queue>
#include <string>

template<class TValue>
class Trie {
public:
    struct NodeContent;
    typedef int Node;
    typedef std::vector<TValue> Value;
    typedef std::vector<Node> Transitions;

    enum { 
        NO_TRANSITION = -1, 
        TRANSITION_COUNT = 30,
        MIN_SYMBOL = 'a', 
        MAX_SYMBOL = MIN_SYMBOL + TRANSITION_COUNT 
    };

    struct NodeContent {
    public:
        Value value_;
        Transitions transitions_;

        NodeContent() :
            value_(),
            transitions_(30) {
            std::fill(transitions_.begin(), transitions_.end(), NO_TRANSITION);
        }
    };

    void AddValue(Node node, TValue value) {
        nodes_[node].value_.push_back(value);
    }

    bool HasValue(Node node) const {
        return nodes_[node].value_.size() != 0;
    }

    const Value &GetValue(Node node) {
        return nodes_[node].value_;
    }

    bool HasTransition(Node node, char symbol) const { 
        const auto &transitions = nodes_[node].transitions_;

        return transitions.at(symbol - MIN_SYMBOL) != NO_TRANSITION;
    }

    const Node GetTransition(Node node, char symbol) const {
        return nodes_[node].transitions_.at(symbol - MIN_SYMBOL);
    }

    void AddTransition(Node node, char symbol, Node transition) {
        nodes_[node].transitions_[symbol - MIN_SYMBOL] = transition;
    }

    Trie() :
        nodes_(),
        root_(CreateEmptyNode()),
        keys_counter_(0)
        {}

    Node CreateEmptyNode() {
        nodes_.emplace_back(NodeContent());
        return nodes_.size() - 1;
    }

    bool IsRoot(Node node) const {
        return node == root_;  
    }

    Node GetRoot() const {
        return root_;
    }

    size_t GetNodesCount() const {
        return nodes_.size();
    }

    void AddKey(const std::string &key, const TValue &value) {
        Node current_node = root_;
        auto current_symbol = key.begin();
 
        while (current_symbol != key.end() &&
               HasTransition(current_node, *current_symbol)) {
            current_node = GetTransition(current_node, *current_symbol);
            ++current_symbol;
        }
   
        while (current_symbol != key.end()) {
            AddTransition(current_node, *current_symbol, CreateEmptyNode());
            current_node = GetTransition(current_node, *current_symbol);
            ++current_symbol;
        }
        
        if (current_symbol == key.end()) {
            AddValue(current_node, value);
        }
    }

    bool HasKey(const std::string &key) const {
        Node current_node = root_;
        auto current_symbol = key.begin();        

        while (current_symbol != key.end() &&
               HasTransition(current_node, *current_symbol)) {
            current_node = GetTransition(current_node, *current_symbol);
            ++current_symbol;
        }
    
        return (current_symbol == key.end() && HasValue(current_node));
    }

private:
    std::vector<NodeContent> nodes_;
    Node root_;

    size_t keys_counter_;
};


class PatternsMatcher {
protected:
    typedef size_t Match;
    typedef Trie<Match> PatternTrie;

public:

    std::vector<std::vector<size_t>> SearchPatterns(std::string string) const {
        std::vector<std::vector<size_t>> matches(pattern_count_);
        PatternTrie::Node node = trie_.GetRoot();

        for (size_t index = 0; index < string.size(); ++index) {
            char symbol = string[index];
            while (GetGotoTransition(node, symbol) == PatternTrie::NO_TRANSITION) {
                node = GetFailTransition(node);
            }

            node = GetGotoTransition(node, symbol);

            for (auto match: GetPatternMatches(node)) {
                matches[match].push_back(index);
            }
        }

        return std::move(matches);
    }

    void Init(std::vector<std::string> patterns) {
        pattern_count_ = patterns.size();
        InitTrie(patterns);

        fail_transitions_.resize(trie_.GetNodesCount());
        pattern_matches_.resize(trie_.GetNodesCount());

        std::fill(fail_transitions_.begin(), fail_transitions_.end(), PatternTrie::NO_TRANSITION);

        InitAutomata(); 
    }

protected:
    PatternTrie::Node GetTransition(PatternTrie::Node node, char symbol) const {
        if (GetGotoTransition(node, symbol) == PatternTrie::NO_TRANSITION) {
            while (GetGotoTransition(node, symbol) == PatternTrie::NO_TRANSITION) {
                node = GetFailTransition(node);
            }

            node = GetGotoTransition(node, symbol);
            return node;
        } else {
            return GetGotoTransition(node, symbol);
        }
    }

private: 
    void InitTrie(const std::vector<std::string> &patterns) {
        for (size_t pattern_index = 0; pattern_index < patterns.size(); ++pattern_index) {
            trie_.AddKey(patterns[pattern_index], pattern_index);
        }
    }

    void InitAutomata() {
        std::queue<PatternTrie::Node> queue;
        auto root = trie_.GetRoot();
        fail_transitions_[root] = root;
        AddPatternMatches(root, std::vector<size_t>()); // in case there are empty patterns

        for (size_t symbol = PatternTrie::MIN_SYMBOL; symbol < PatternTrie::MAX_SYMBOL; ++symbol) {
            if (trie_.HasTransition(trie_.GetRoot(), symbol)) {
                auto node = trie_.GetTransition(trie_.GetRoot(), symbol);
    
                fail_transitions_[node] = root;
                AddPatternMatches(node, GetPatternMatches(root));

                queue.push(node);
            }
        }

        while (!queue.empty()) {
            auto current_node = queue.front();
            queue.pop();
            
            for (size_t symbol = PatternTrie::MIN_SYMBOL; 
                    symbol != PatternTrie::MAX_SYMBOL; 
                    ++symbol) {
                if (trie_.HasTransition(current_node, symbol)) {
                    auto transition_from = trie_.GetTransition(current_node, symbol);

                    queue.push(transition_from);
                    auto transition_to = GetFailTransition(current_node);

                    while (GetGotoTransition(transition_to, symbol) == PatternTrie::NO_TRANSITION) {
                        transition_to = GetFailTransition(transition_to);
                    }

                    fail_transitions_[transition_from] = GetGotoTransition(transition_to, symbol);
                    AddPatternMatches(transition_from, 
                            GetPatternMatches(fail_transitions_[transition_from]));
                }
            }
        }
    }

    void AddPatternMatches(PatternTrie::Node node, const std::vector<size_t> &implicit_matches) {
        std::vector<size_t> matches = implicit_matches;

        if (trie_.HasValue(node)) {
            const auto &values = trie_.GetValue(node);
            matches.insert(matches.begin(), values.begin(), values.end());
        }

        pattern_matches_[node] = matches;
    } 

protected: 
    PatternTrie::Node GetRoot() const {
        return trie_.GetRoot();
    }

    bool IsTerminalState(PatternTrie::Node node) const {
        return (pattern_matches_.at(node).size() > 0);
    }

    PatternTrie::Node GetGotoTransition(PatternTrie::Node node, char symbol) const {
        if (trie_.HasTransition(node, symbol)) {
            return trie_.GetTransition(node, symbol);
        } else {
            if (trie_.IsRoot(node)) {
                return node;
            } else {
                return PatternTrie::NO_TRANSITION;   
            }
        }
    }

    PatternTrie::Node GetFailTransition(PatternTrie::Node node) const {
        if (fail_transitions_.at(node) != PatternTrie::NO_TRANSITION) {
            return fail_transitions_.at(node);
        } else {
            return trie_.GetRoot();
        }
    }

    std::vector<size_t> GetPatternMatches(PatternTrie::Node node) const {
        return pattern_matches_.at(node);
    }

    size_t GetNodesCount() const {
        return trie_.GetNodesCount();
    }

private:
    PatternTrie trie_;

    std::vector<PatternTrie::Node> fail_transitions_;
    std::vector<std::vector<size_t>> pattern_matches_;
    int pattern_count_;
};

class StringsCounter : public PatternsMatcher {
private:
    typedef std::map<PatternTrie::Node, long long int> DynamicTableLevel;
    typedef std::vector<DynamicTableLevel> DynamicTable;

public:
    static const int MODULUS = 1000000007;

    long long int CountExcludedStrings(size_t max_word_length, 
            size_t alphabet_size) {
        DynamicTable table(max_word_length + 1);
         
        size_t nodes_count = GetNodesCount();
        
        for (size_t node = 0; node < nodes_count; ++node) {
            if (IsTerminalState(node)) {
                for (size_t word_length = 0; word_length <= max_word_length; ++word_length) {
                    table[word_length][node] = 0;
                }
            } else {
                table[0][node] = 1;
            }
        }  

        for (size_t word_length = 1; word_length <= max_word_length; ++word_length) {
            for (size_t node = 0; node < nodes_count; ++node) {
                if (!IsTerminalState(node)) {
                    long long int result = 0;

                    for (char symbol_index = 0; symbol_index < alphabet_size; ++symbol_index) {
                        result += table[word_length - 1][GetTransition(node, 'a' + symbol_index)];
                        result %= MODULUS;
                    }

                    table[word_length][node] = result;
                }
            } 
        }

        return table[max_word_length][GetRoot()];
    }
};
