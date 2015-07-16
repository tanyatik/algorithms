#pragma once

#include <string>
#include <map>
#include <vector>
#include <deque>

class SuffixAutomata {
public:
    SuffixAutomata();

    void Init(const std::string& string);
    void Init();
    long long CountDifferentSubstrings() const;

private:
    struct State {
        static const char MIN_SYMBOL = 'a';

        typedef std::map<char, int> Jumps;
        int length;
        int link;
        int firstpos;
        int strings_counter;

        int GetJump(char symbol) const {
            return jumps_.at(symbol - MIN_SYMBOL);
        }

        const Jumps &GetJumps() const {
            return jumps_;
        }

        void SetJump(char symbol, int state) {
            jumps_[symbol - MIN_SYMBOL] = state;
        }

        void ClearJump(char symbol) {
            jumps_.erase(symbol - MIN_SYMBOL);
        }

        bool HasJump(char symbol) const {
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

    void Extend(char symbol, int* last, int* sz);

    void AddJump(int from, int to, char symbol);

    void RemoveJump(int from, int to, char symbol);

    void Clone(int templ, int changeable);

    long long CountDifferentSubstringsRecursively(size_t current_state,
            std::vector<long long> *cache,
            int invalid_jump) const {
        long long sum = 0L;
        for (const auto &jump: states_.at(current_state).GetJumps()) {
            if ((*cache).at(jump.second) == invalid_jump) {
                CountDifferentSubstringsRecursively(jump.second, cache, invalid_jump);
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

void SuffixAutomata::Init(const std::string& string) {
    int last_empty_index = 1;

    states_.resize(std::max(2*string.size(), 10LU));
    for (std::string::const_iterator it = string.begin(); it != string.end(); ++it) {
        Extend(*it, &last_state_, &last_empty_index);
    }
}

void SuffixAutomata::AddJump(int from, int to, char symbol) {
    states_[from].SetJump(symbol, to);
}

void SuffixAutomata::RemoveJump(int from, int to, char symbol) {
    states_[from].ClearJump(symbol);
}

void SuffixAutomata::Clone(int new_state, int old_state) {
    states_[new_state] = states_[old_state];
}

void SuffixAutomata::Extend(char symbol, int* last, int* size) {
    // create new state "current"
    // and init it's length and firstpos
    int cur_index = *size;
    (*size)++;
    states_[cur_index].length = states_[*last].length + 1;
    states_[cur_index].firstpos = states_[cur_index].length - 1;

    int position = 0;
    for (position = *last;
            position != -1 && !states_[position].HasJump(symbol);
            position = states_[position].link) {
        AddJump(position, cur_index, symbol);
    }
    // if there was no such symbol in string
    if (position == -1) {
        states_[cur_index].link = 0;
    } else {
        // there already was symbol in string
        int to_clone = states_[position].GetJump(symbol);
        if (states_[position].length + 1 == states_[to_clone].length) {
            states_[cur_index].link = to_clone;
        } else {
            // splitting the state
            int cloneable = *size;
            (*size)++;
            Clone(cloneable, to_clone);
            states_[cloneable].length = states_[position].length + 1;

            // change jumps to cloned state
            for (; position != -1 && states_[position].GetJump(symbol) == to_clone;
                    position = states_[position].link) {
                RemoveJump(position, to_clone, symbol);
                AddJump(position, cloneable, symbol);
            }
            // change links to cloned state
            states_[to_clone].link = states_[cur_index].link = cloneable;
        }
    }
    *last = cur_index;
}

long long SuffixAutomata::CountDifferentSubstrings() const {
    size_t invalid_jump = -1;
    std::vector<long long> cache(states_.size(), invalid_jump);
    long long result = CountDifferentSubstringsRecursively(0, &cache, invalid_jump) - 1;

    return result;
}

