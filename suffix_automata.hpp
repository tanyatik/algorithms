#pragma once

#include <string>
#include <map>
#include <vector>
#include <deque>

class SuffixAutomata {
public:
    SuffixAutomata();

    void init(const std::string& string);
    void init();
    long long countDifferentSubstrings() const;

private:
    struct State {
        static const char MIN_SYMBOL = 'a';

        typedef std::map<char, int> Jumps;
        int length;
        int link;
        int firstpos;
        int strings_counter;

        int getJump(char symbol) const {
            return jumps_.at(symbol - MIN_SYMBOL);
        }
    
        const Jumps &getJumps() const {
            return jumps_;
        } 

        void setJump(char symbol, int state) {
            jumps_[symbol - MIN_SYMBOL] = state;
        }

        void clearJump(char symbol) {
            jumps_.erase(symbol - MIN_SYMBOL);
        }

        bool hasJump(char symbol) const {
            return jumps_.find(symbol - MIN_SYMBOL) != jumps_.end();
        }
        
        State() :
            length(0),
            link(-1),
            firstpos(0),
            strings_counter(0) {
        }

    private:
        Jumps jumps_;
    };

    void extend(char symbol, int* last, int* sz);

    void addJump(int from, int to, char symbol);

    void removeJump(int from, int to, char symbol);

    void clone(int templ, int changeable);

    long long countDifferentSubstringsRecursively(size_t current_state, 
            std::vector<long long> *cache,
            int invalid_jump) const {
        long long sum = 0L; 
        for (const auto &jump: states_.at(current_state).getJumps()) {
            if ((*cache).at(jump.second) == invalid_jump) {
                countDifferentSubstringsRecursively(jump.second, cache, invalid_jump); 
            }
            sum += (*cache)[jump.second];
        }

        ++sum;

        (*cache)[current_state] = sum;
        return sum;
    }

    typedef std::vector<State> States;
    States states_;
    int last_state_;
};

SuffixAutomata::SuffixAutomata() {
    State zero_state;
    zero_state.length = 0;
    zero_state.link = -1;
    zero_state.firstpos = 0;
    zero_state.strings_counter = 0;
    states_.push_back(zero_state);

    last_state_ = 0;
}

void SuffixAutomata::init(const std::string& string) {
    int last_empty_index = 1;

    states_.resize(std::max(2*string.size(), 10LU));
    for (std::string::const_iterator it = string.begin(); it != string.end(); ++it) {
        extend(*it, &last_state_, &last_empty_index);
    }
}

void SuffixAutomata::addJump(int from, int to, char symbol) {
    states_[from].setJump(symbol, to);
}

void SuffixAutomata::removeJump(int from, int to, char symbol) {
    states_[from].clearJump(symbol);
}

void SuffixAutomata::clone(int new_state, int old_state) {
    states_[new_state] = states_[old_state];
}

void SuffixAutomata::extend(char symbol, int* last, int* size) {
    // create new state "current"
    // and init it's length and firstpos
    int cur_index = *size;
    (*size)++;
    states_[cur_index].length = states_[*last].length + 1;
    states_[cur_index].firstpos = states_[cur_index].length - 1;

    int position = 0;
    for (position = *last; 
            position != -1 && !states_[position].hasJump(symbol); 
            position = states_[position].link) {
        addJump(position, cur_index, symbol);
    }
    // if there was no such symbol in string
    if (position == -1) {
        states_[cur_index].link = 0;
    } else {
        // there already was symbol in string 
        int to_clone = states_[position].getJump(symbol);
        if (states_[position].length + 1 == states_[to_clone].length) {
            states_[cur_index].link = to_clone;
        } else {
            // splitting the state
            int cloneable = *size;
            (*size)++;
            clone(cloneable, to_clone);
            states_[cloneable].length = states_[position].length + 1;

            // change jumps to cloned state
            for (; position != -1 && states_[position].getJump(symbol) == to_clone; 
                    position = states_[position].link) {
                removeJump(position, to_clone, symbol);
                addJump(position, cloneable, symbol);
            }
            // change links to cloned state
            states_[to_clone].link = states_[cur_index].link = cloneable;
        }
    }
    *last = cur_index;
}

long long SuffixAutomata::countDifferentSubstrings() const {
    size_t invalid_jump = -1;
    std::vector<long long> cache(states_.size(), invalid_jump);
    long long result = countDifferentSubstringsRecursively(0, &cache, invalid_jump) - 1;

    return result;
}

