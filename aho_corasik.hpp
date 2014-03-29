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
    typedef std::shared_ptr<NodeContent> Node;
    typedef std::vector<TValue> Value;
    typedef std::map<char, Node> Transitions;

    struct NodeContent {
    public:
        void addValue(TValue value) {
            value_.push_back(value);
        }

        bool hasValue() const {
            return value_.size() != 0;
        }

        const Value &getValue() {
            return value_;
        }

        bool hasTransition(char symbol) const {
            return transitions_.find(symbol) != transitions_.end();
        }

        const Node getTransition(char symbol) const {
            return transitions_.at(symbol);
        }

        void addTransition(char symbol, Node transition) {
            transitions_[symbol] = transition;
        }

        const Transitions &getTransitions() const {
            return transitions_;
        }
    
    private:
        Value value_;
        Transitions transitions_;
    };

    Trie() :
        root_(createEmptyNode()),
        keys_counter_(0)
        {}

    Node createEmptyNode() const {
        return std::make_shared<NodeContent>(NodeContent());
    }

    bool isRoot(Node node) const {
        return node == root_;  
    }

    Node getRoot() const {
        return root_;
    }

    void addKey(const std::string &key, const TValue &value) {
        Node current_node = root_;
        auto current_symbol = key.begin();
 
        while (current_symbol != key.end() &&
               current_node->hasTransition(*current_symbol)) {
            current_node = current_node->getTransition(*current_symbol);
            ++current_symbol;
        }
   
        while (current_symbol != key.end()) {
            current_node->addTransition(*current_symbol, createEmptyNode());
            current_node = current_node->getTransition(*current_symbol);
            ++current_symbol;
        }
        
        if (current_symbol == key.end()) {
            current_node->addValue(value);
        }
    }

    bool hasKey(const std::string &key) const {
        Node current_node = root_;
        auto current_symbol = key.begin();        

        while (current_symbol != key.end() &&
               current_node->hasTransition(*current_symbol)) {
            current_node = current_node->getTransition(*current_symbol);
            ++current_symbol;
        }
    
        return (current_symbol == key.end() && current_node->hasValue());
    }

private:
    Node root_;
    size_t keys_counter_;
};


class PatternsMatcher {
private:
    typedef size_t Match;
    typedef Trie<Match> PatternTrie;

public:
    std::vector<std::vector<size_t>> searchPatterns(std::string string) const {
        std::vector<std::vector<size_t>> matches(pattern_count_);
        PatternTrie::Node node = trie_.getRoot();

        for (size_t index = 0; index < string.size(); ++index) {
            while (getGotoTransition(node, string[index]) == nullptr) {
                node = getFailTransition(node);
            }

            node = getGotoTransition(node, string[index]);
            
            for (auto match: getPatternMatches(node)) {
                matches[match].push_back(index);
            }
        }

        return std::move(matches);
    }

    void init(std::vector<std::string> patterns) {
        pattern_count_ = patterns.size();
        initTrie(patterns);
        initAutomata(); 
    }

    size_t getDebugStateNumber(PatternTrie::Node node, 
            std::map<PatternTrie::Node, size_t> *node_numbers, 
            size_t *node_counter) {
        auto found = node_numbers->find(node);
        if (found == node_numbers->end()) {
            (*node_numbers)[node] = ++*node_counter;
        }
        
        return (*node_numbers)[node];
    }

    void debugPrint() {
        std::queue<PatternTrie::Node> queue;
        queue.push(trie_.getRoot());
        std::map<PatternTrie::Node, size_t> node_numbers;

        size_t node_counter = 0;
        node_numbers[queue.front()] = node_counter;

        while (!queue.empty()) {
            auto current_state = queue.front();
            size_t current_state_number = 
                getDebugStateNumber(current_state, &node_numbers, &node_counter);
            queue.pop();

            std::cout << current_state_number << "\tfail: " <<
                getDebugStateNumber(getFailTransition(current_state), &node_numbers, &node_counter);

            std::cout << "\tgoto: ";

            for (const auto &transition: current_state->getTransitions()) {
                auto symbol = transition.first;
                auto state = transition.second; 

                size_t state_number = getDebugStateNumber(state, &node_numbers, &node_counter);

                std::cout << " { " << symbol << " -> " << state_number << " } ";
                queue.push(state);
            }
            std::cout << "\tmatches: { ";
            for (const auto &match: pattern_matches_[current_state]) { 
                std::cout << match << " ";
            }
            std::cout << " } ";
            std::cout << std::endl;
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

        for (const auto &node: trie_.getRoot()->getTransitions()) {
            auto state = node.second; 
    
            fail_transitions_[state] = root;
            addPatternMatches(state, std::vector<size_t>());

            queue.push(state);
        }

        while (!queue.empty()) {
            auto current_node = queue.front();
            queue.pop();
            
            for (const auto &transition: current_node->getTransitions()) {
                auto symbol = transition.first;
                auto transition_from = transition.second;

                queue.push(transition_from);
                auto transition_to = getFailTransition(current_node);

                while (getGotoTransition(transition_to, symbol) == nullptr) {
                    transition_to = getFailTransition(transition_to);
                }

                fail_transitions_[transition_from] = getGotoTransition(transition_to, symbol);
                addPatternMatches(transition_from, 
                        getPatternMatches(fail_transitions_[transition_from]));
            }
        }
    }

    PatternTrie::Node getGotoTransition(PatternTrie::Node node, char symbol) const {
        if (node->hasTransition(symbol)) {
            return node->getTransition(symbol);
        } else {
            if (trie_.isRoot(node)) {
                return node;
            } else {
                return nullptr;   
            }
        }
    }

    PatternTrie::Node getFailTransition(PatternTrie::Node node) const {
        auto found = fail_transitions_.find(node);
        if (found != fail_transitions_.end()) {
            return found->second;
        } else {
            return trie_.getRoot();
        }
    }

    void addPatternMatches(PatternTrie::Node node, const std::vector<size_t> &implicit_matches) {
        std::vector<size_t> matches = implicit_matches;

        if (node->hasValue()) {
            const auto &values = node->getValue();
            matches.insert(matches.begin(), values.begin(), values.end());
        }

        pattern_matches_[node] = matches;
    } 

    std::vector<size_t> getPatternMatches(PatternTrie::Node node) const {
        auto found = pattern_matches_.find(node);
        if (found != pattern_matches_.end()) {
            return found->second;
        }
        
        return std::vector<size_t>();
    }

    PatternTrie trie_;

    std::map<PatternTrie::Node, PatternTrie::Node> goto_transitions_;
    std::map<PatternTrie::Node, PatternTrie::Node> fail_transitions_;
    std::map<PatternTrie::Node, std::set<size_t>> pattern_matches_;

    int pattern_count_;
};

