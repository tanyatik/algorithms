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

    void addValue(Node node, TValue value) {
        nodes_[node].value_.push_back(value);
    }

    bool hasValue(Node node) const {
        return nodes_[node].value_.size() != 0;
    }

    const Value &getValue(Node node) {
        return nodes_[node].value_;
    }

    bool hasTransition(Node node, char symbol) const { 
        const auto &transitions = nodes_[node].transitions_;

        return transitions.at(symbol - MIN_SYMBOL) != NO_TRANSITION;
    }

    const Node getTransition(Node node, char symbol) const {
        return nodes_[node].transitions_.at(symbol - MIN_SYMBOL);
    }

    void addTransition(Node node, char symbol, Node transition) {
        nodes_[node].transitions_[symbol - MIN_SYMBOL] = transition;
    }

    Trie() :
        nodes_(),
        root_(createEmptyNode()),
        keys_counter_(0)
        {}

    Node createEmptyNode() {
        nodes_.emplace_back(NodeContent());
        return nodes_.size() - 1;
    }

    bool isRoot(Node node) const {
        return node == root_;  
    }

    Node getRoot() const {
        return root_;
    }

    size_t getNodesCount() const {
        return nodes_.size();
    }

    void addKey(const std::string &key, const TValue &value) {
        Node current_node = root_;
        auto current_symbol = key.begin();
 
        while (current_symbol != key.end() &&
               hasTransition(current_node, *current_symbol)) {
            current_node = getTransition(current_node, *current_symbol);
            ++current_symbol;
        }
   
        while (current_symbol != key.end()) {
            addTransition(current_node, *current_symbol, createEmptyNode());
            current_node = getTransition(current_node, *current_symbol);
            ++current_symbol;
        }
        
        if (current_symbol == key.end()) {
            addValue(current_node, value);
        }
    }

    bool hasKey(const std::string &key) const {
        Node current_node = root_;
        auto current_symbol = key.begin();        

        while (current_symbol != key.end() &&
               hasTransition(current_node, *current_symbol)) {
            current_node = getTransition(current_node, *current_symbol);
            ++current_symbol;
        }
    
        return (current_symbol == key.end() && hasValue(current_node));
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

    std::vector<std::vector<size_t>> searchPatterns(std::string string) const {
        std::vector<std::vector<size_t>> matches(pattern_count_);
        PatternTrie::Node node = trie_.getRoot();

        for (size_t index = 0; index < string.size(); ++index) {
            char symbol = string[index];
            while (getGotoTransition(node, symbol) == PatternTrie::NO_TRANSITION) {
                node = getFailTransition(node);
            }

            node = getGotoTransition(node, symbol);

            for (auto match: getPatternMatches(node)) {
                matches[match].push_back(index);
            }
        }

        return std::move(matches);
    }

    void init(std::vector<std::string> patterns) {
        pattern_count_ = patterns.size();
        initTrie(patterns);

        fail_transitions_.resize(trie_.getNodesCount());
        pattern_matches_.resize(trie_.getNodesCount());

        std::fill(fail_transitions_.begin(), fail_transitions_.end(), PatternTrie::NO_TRANSITION);

        initAutomata(); 
    }

protected:
    PatternTrie::Node getTransition(PatternTrie::Node node, char symbol) const {
        if (getGotoTransition(node, symbol) == PatternTrie::NO_TRANSITION) {
            while (getGotoTransition(node, symbol) == PatternTrie::NO_TRANSITION) {
                node = getFailTransition(node);
            }

            node = getGotoTransition(node, symbol);
            return node;
        } else {
            return getGotoTransition(node, symbol);
        }
    }

private: 
    void initTrie(const std::vector<std::string> &patterns) {
        for (size_t pattern_index = 0; pattern_index < patterns.size(); ++pattern_index) {
            trie_.addKey(patterns[pattern_index], pattern_index);
        }
    }

    void initAutomata() {
        std::queue<PatternTrie::Node> queue;
        auto root = trie_.getRoot();
        fail_transitions_[root] = root;
        addPatternMatches(root, std::vector<size_t>()); // in case there are empty patterns

        for (size_t symbol = PatternTrie::MIN_SYMBOL; symbol < PatternTrie::MAX_SYMBOL; ++symbol) {
            if (trie_.hasTransition(trie_.getRoot(), symbol)) {
                auto node = trie_.getTransition(trie_.getRoot(), symbol);
    
                fail_transitions_[node] = root;
                addPatternMatches(node, getPatternMatches(root));

                queue.push(node);
            }
        }

        while (!queue.empty()) {
            auto current_node = queue.front();
            queue.pop();
            
            for (size_t symbol = PatternTrie::MIN_SYMBOL; symbol != PatternTrie::MAX_SYMBOL; ++symbol) {
                if (trie_.hasTransition(current_node, symbol)) {
                    auto transition_from = trie_.getTransition(current_node, symbol);

                    queue.push(transition_from);
                    auto transition_to = getFailTransition(current_node);

                    while (getGotoTransition(transition_to, symbol) == PatternTrie::NO_TRANSITION) {
                        transition_to = getFailTransition(transition_to);
                    }

                    fail_transitions_[transition_from] = getGotoTransition(transition_to, symbol);
                    addPatternMatches(transition_from, 
                            getPatternMatches(fail_transitions_[transition_from]));
                }
            }
        }
    }

    void addPatternMatches(PatternTrie::Node node, const std::vector<size_t> &implicit_matches) {
        std::vector<size_t> matches = implicit_matches;

        if (trie_.hasValue(node)) {
            const auto &values = trie_.getValue(node);
            matches.insert(matches.begin(), values.begin(), values.end());
        }

        pattern_matches_[node] = matches;
    } 

protected: PatternTrie::Node getRoot() const {
        return trie_.getRoot();
    }

    bool isTerminalState(PatternTrie::Node node) const {
        return (pattern_matches_.at(node).size() > 0);
    }

    PatternTrie::Node getGotoTransition(PatternTrie::Node node, char symbol) const {
        if (trie_.hasTransition(node, symbol)) {
            return trie_.getTransition(node, symbol);
        } else {
            if (trie_.isRoot(node)) {
                return node;
            } else {
                return PatternTrie::NO_TRANSITION;   
            }
        }
    }

    PatternTrie::Node getFailTransition(PatternTrie::Node node) const {
        if (fail_transitions_.at(node) != PatternTrie::NO_TRANSITION) {
            return fail_transitions_.at(node);
        } else {
            return trie_.getRoot();
        }
    }

    std::vector<size_t> getPatternMatches(PatternTrie::Node node) const {
        return pattern_matches_.at(node);
    }

    size_t getNodesCount() const {
        return trie_.getNodesCount();
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

    long long int countExcludedStrings(size_t max_word_length, 
            size_t alphabet_size) {
        DynamicTable table(max_word_length + 1);
         
        size_t nodes_count = getNodesCount();
        
        for (size_t node = 0; node < nodes_count; ++node) {
            if (isTerminalState(node)) {
                for (size_t word_length = 0; word_length <= max_word_length; ++word_length) {
                    table[word_length][node] = 0;
                }
            } else {
                table[0][node] = 1;
            }
        }  

        for (size_t word_length = 1; word_length <= max_word_length; ++word_length) {
            for (size_t node = 0; node < nodes_count; ++node) {
                if (!isTerminalState(node)) {
                    long long int result = 0;

                    for (char symbol_index = 0; symbol_index < alphabet_size; ++symbol_index) {
                        result += table[word_length - 1][getTransition(node, 'a' + symbol_index)];
                        result %= MODULUS;
                    }

                    table[word_length][node] = result;
                }
            } 
        }

        return table[max_word_length][getRoot()];
    }
};
