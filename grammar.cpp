#include <vector>
#include <stdexcept>
#include <iostream>
#include <set>
#include <string>

class Grammar {
public:
    static constexpr char EPSILON = '$';

    typedef int Symbol;
    typedef std::vector<Symbol> Production;

    static constexpr int ERROR_NONTERMINAL = -1;

    enum SymbolType {
        TERMINAL,
        NONTERMINAL,
        SENTINEL
    };

private:
    std::vector<std::vector<Production>> productions_table_;
    std::vector<SymbolType> symbol_types_;
    Symbol goal_;

    void addNonterminal(Symbol nonterminal);

public:
    Grammar() :
        productions_table_(),
        goal_(ERROR_NONTERMINAL)
        {}

    void setGoal(Symbol goal) {
        goal_ = goal;
    }
    
    Symbol getGoal() const {
        return goal_;
    }

    void setSymbolType(Symbol symbol, SymbolType type) {
        if (symbol_types_.size() <= symbol) {
            symbol_types_.resize(symbol + 1);
        }
        symbol_types_.at(symbol) = type;
    }

    bool isTerminal(Symbol symbol) const {
        return symbol_types_.at(symbol) == TERMINAL;
    } 

    bool isNonterminal(Symbol symbol) const {
        return symbol_types_.at(symbol) == NONTERMINAL;
    }

    bool isSentinel(Symbol symbol) const {
        return symbol_types_.at(symbol) == SENTINEL;
    }

    size_t getNonterminalsCount() const;
    Symbol makeNonterminal(); 

    void addProduction(Symbol symbol, const Production &production);
    void removeProduction(Symbol symbol, size_t production_index) {
        auto &productions = productions_table_.at(symbol);
        productions.at(production_index).clear();
        // productions.erase(productions.begin() + production_index);
    }

    size_t getProductionsCount(Symbol symbol) const;
    const Production &getProduction(Symbol symbol, size_t production_index) const;
    void changeProduction(Symbol symbol, size_t production_index, Production &&production) {
        productions_table_.at(symbol).at(production_index) = production;
    }
    bool isEpsilonProduction(const Production &production) const;

    void debugPrintNonterminal(Symbol symbol) const;
    void debugPrintProduction(const Production &production) const;
    void debugPrint() const;

    bool isChomsky() const;
};

class GrammarReader {
private:
    static constexpr char FIRST_TERMINAL = 'a';
    static constexpr char LAST_TERMINAL = 'z'; 
    static constexpr char FIRST_NONTERMINAL = 'A';
    static constexpr char LAST_NONTERMINAL = 'Z'; 
    static constexpr char NONTERMINAL_GOAL_LETTER = 'S';
    enum { NOT_FILLED = -1 };

    std::vector<int> nonterminal_map_;

    Grammar::Symbol getNonterminal(Grammar *grammar, char letter);
    void readProductionFromString(Grammar *grammar, const std::string &production_string);

public:
    GrammarReader() :
        nonterminal_map_(LAST_NONTERMINAL - FIRST_NONTERMINAL + 1, NOT_FILLED)
        {}

    Grammar readGrammar(std::istream &stream = std::cin);
};

class ProductionNonterminalizer {
public:
    static void nonterminalizeProductions(Grammar *grammar);
};

class LongProductionsRemover {
private:
    static void addShortProductions(Grammar *grammar, 
            Grammar::Symbol nonterminal, 
            const Grammar::Production &production);

public:
    static void removeLongProductions(Grammar *grammar);
};

class EpsilonProducingProductionsRemover {
private:
    static void addEpsilonInducingGoal(Grammar *grammar);
    static std::vector<bool> getEpsilonProducingNonterminals(Grammar *grammar, bool *induces_any);
    static std::vector<bool> getEpsilonInducingNonterminals(Grammar *grammar, bool *induces_any);
    static void addEpsilonRemovedProductions(Grammar *grammar,
            Grammar::Production head,
            std::vector<Grammar::Symbol>::iterator tail_begin, 
            std::vector<Grammar::Symbol>::iterator tail_end,
            Grammar::Symbol nonterminal,
            std::vector<bool> nonterminal_induces_epsilon);
    static void removeEpsilonProductions(Grammar *grammar);
    
public:
    static void removeEpsilonProducingProductions(Grammar *grammar);
};

class ChainProductionsRemover {
private:
    struct ChainedPair {
        Grammar::Symbol parent;  
        Grammar::Symbol child;

        ChainedPair(Grammar::Symbol parent, Grammar::Symbol child) :
            parent(parent),
            child(child)
            {}
        bool operator < (const ChainedPair &other) const {
            return (parent < other.parent) || 
                   (parent == other.parent && child < other.child);
        }
    };

    struct ChainedProduction {
        Grammar::Symbol nonterminal;
        size_t production_index;
        
        ChainedProduction(Grammar::Symbol nonterminal, size_t production_index) :
            nonterminal(nonterminal),
            production_index(production_index)
            {}
    };

    static std::set<ChainedPair> getChainedPairs(Grammar *grammar, 
            std::vector<ChainedProduction> *chained_productions);
    static void removeChainProductions(Grammar *grammar,
            const std::vector<ChainedProduction> &chained_productions);
    static bool isChainedProduction(Grammar *grammar, const Grammar::Production &production);

public:
    static void removeChains(Grammar *grammar);
};

class ShortestStringFinder {
private:
    const Grammar &grammar_;
    std::vector<std::vector<std::string>> shortest_strings_;
    size_t max_length_;
   
    void initShortestStrings(); 
    void computeShortestStrings(); 
    std::string computeShortestStringForNonterminal
            (Grammar::Symbol nonterminal, size_t length);
    std::string getComposedString(Grammar::Symbol nonterminal_left, 
            Grammar::Symbol nonterminal_right, 
            size_t left_part_size,
            size_t length) const;

    bool grammarProducesEpsilon() const;

public:
    ShortestStringFinder(const Grammar &grammar, size_t max_length) :
        grammar_(grammar),
        shortest_strings_
            (grammar.getNonterminalsCount(), std::vector<std::string>(max_length + 1, "")),
        max_length_(max_length)
        {}

    std::string getShortestString();

    void debugPrint() {
        for (int nonterminal_index = 0; 
                nonterminal_index < shortest_strings_.size(); 
                ++nonterminal_index) {
            for (int length = 0; length <= max_length_; ++length) {
                std::cerr << "F[";
                grammar_.debugPrintNonterminal(nonterminal_index);
                std::cerr << "][" <<  length << "]=<" << 
                        shortest_strings_.at(nonterminal_index).at(length) << "> ";
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
};


int readMaximumLength(std::istream &stream = std::cin);

void writeShortestString(const std::string &string, std::ostream &stream = std::cout);

// main

int main() {
    GrammarReader reader;
    Grammar grammar = reader.readGrammar();
    int maximum_length = readMaximumLength();

#ifdef DEBUG 
    std::cerr << "-----Source-----\n";
    grammar.debugPrint();
#endif

    ProductionNonterminalizer::nonterminalizeProductions(&grammar);
    LongProductionsRemover::removeLongProductions(&grammar);
    
#ifdef DEBUG 
    std::cerr << "-----Long productions removed-----\n";
    grammar.debugPrint();
#endif

    EpsilonProducingProductionsRemover::removeEpsilonProducingProductions(&grammar);

#ifdef DEBUG 
    std::cerr << "-----Epsilon producing productions removed-----\n";
    grammar.debugPrint();
#endif

    ChainProductionsRemover::removeChains(&grammar);

#ifdef DEBUG 
    std::cerr << "-----Chains removed-----\n";
    grammar.debugPrint();
#endif

#ifdef DEBUG
    assert(grammar.isChomsky());
#endif

    ShortestStringFinder finder(grammar, maximum_length);

    auto shortest_string = finder.getShortestString();  

    writeShortestString(shortest_string);

    return 0;
}

// Grammar implementation

size_t Grammar::getNonterminalsCount() const {
    return productions_table_.size();
}

void Grammar::addNonterminal(Symbol nonterminal) {
    if (productions_table_.size() <= nonterminal) {
        productions_table_.resize(nonterminal + 1);
    }

    setSymbolType(nonterminal, NONTERMINAL);        
}

Grammar::Symbol Grammar::makeNonterminal() {
    Symbol new_nonterminal = productions_table_.size();
    addNonterminal(new_nonterminal);
    return new_nonterminal;
}

void Grammar::addProduction(Symbol symbol, const Production &production) {
    productions_table_.at(symbol).push_back(production);
}

size_t Grammar::getProductionsCount(Symbol symbol) const {
    return productions_table_.at(symbol).size(); 
} 

const Grammar::Production &Grammar::getProduction(Symbol symbol, size_t production_index) const {
    return productions_table_.at(symbol).at(production_index);
}

bool Grammar::isEpsilonProduction(const Grammar::Production &production) const {
    bool result = production.size() == 1 && production.at(0) == EPSILON;
    return result;
}

Grammar::Symbol GrammarReader::getNonterminal(Grammar *grammar, char symbol) {
    size_t symbol_index = symbol - FIRST_NONTERMINAL;
    
    if (nonterminal_map_.at(symbol_index) == NOT_FILLED) {
        nonterminal_map_.at(symbol_index) = grammar->makeNonterminal();
        if (symbol == NONTERMINAL_GOAL_LETTER) {
            grammar->setGoal(nonterminal_map_.at(symbol_index));
        }
    } 

    return nonterminal_map_.at(symbol_index);
}

void GrammarReader::readProductionFromString(Grammar *grammar, 
        const std::string &production_string) {
    auto symbol = production_string.begin();
    if (!(FIRST_NONTERMINAL <= *symbol && *symbol <= LAST_NONTERMINAL)) {
        throw std::runtime_error("Wrong symbol in production (should be nonterminal): " + 
                std::to_string(*symbol));
    }
    auto left_nonterminal = getNonterminal(grammar, *symbol);
    ++symbol;

    if (*symbol++ != '-') {
        throw std::runtime_error("Wrong symbol in production: (should be -)" + 
                std::to_string(*symbol));
    } 
    if (*symbol++ != '>') {
        throw std::runtime_error("Wrong symbol in production: (should be >)" + 
                std::to_string(*symbol));
    } 

    Grammar::Production production;
   
    for (; symbol != production_string.end() && *symbol != '\0'; ++symbol) {
        if (FIRST_TERMINAL <= *symbol && *symbol <= LAST_TERMINAL) {
            grammar->setSymbolType(*symbol, Grammar::TERMINAL);
            production.push_back(*symbol); 
        } else if (Grammar::EPSILON == *symbol) {
            grammar->setSymbolType(*symbol, Grammar::SENTINEL);
            production.push_back(*symbol);
        } else if (FIRST_NONTERMINAL <= *symbol && *symbol <= LAST_NONTERMINAL) {
            Grammar::Symbol nonterminal = getNonterminal(grammar, *symbol);
            production.push_back(nonterminal);
        } else {
            throw std::runtime_error("Wrong *symbol in production: " + std::to_string(*symbol));
        }
    } 

    grammar->addProduction(left_nonterminal, production);
}

Grammar GrammarReader::readGrammar(std::istream &stream) {
    Grammar grammar;

    size_t total_productions_count; 
    stream >> total_productions_count; 

    for (auto production_index = 0; 
            production_index < total_productions_count; 
            ++production_index) {
        std::string string;
        stream >> string;

        std::vector<int> nonterminal_map(LAST_NONTERMINAL - FIRST_NONTERMINAL + 1);
        readProductionFromString(&grammar, string);
    }

    if (grammar.getGoal() == grammar.ERROR_NONTERMINAL) {
        throw std::runtime_error("No goal in grammar\n");
    }   
 
    return grammar;
}

int readMaximumLength(std::istream &stream) {
    int max_length;
    stream >> max_length;
    return max_length;
}

void writeShortestString(const std::string &string, std::ostream &stream) {
    if (!string.empty()) {
        stream << string << std::endl;
    } else {
        stream << "IMPOSSIBLE" << std::endl;
    }
}

void Grammar::debugPrintNonterminal(Symbol symbol) const {
    if (symbol == getGoal()) {
        std::cerr << "[";
    }
    if (symbol >= 26) {
        std::cerr << "A";
        std::cerr << static_cast<int>(symbol - 26);
    } else {
        std::cerr << static_cast<char>(symbol + 'A');
    }
    if (symbol == getGoal()) {
        std::cerr << "]";
    }
}

void Grammar::debugPrintProduction(const Production &production) const {
    for (auto symbol: production) {
        if (isNonterminal(symbol)) {
            debugPrintNonterminal(symbol);
        } else {
            std::cerr << static_cast<char>(symbol);
        }
        std::cerr << " ";
    } 
}

void Grammar::debugPrint() const {
    for (int nonterminal = 0; 
            nonterminal < getNonterminalsCount(); 
            ++nonterminal) {

        for (int production_index = 0; 
                production_index < getProductionsCount(nonterminal);
                ++production_index) {
            auto production = getProduction(nonterminal, production_index);

            if (!production.empty()) {
                debugPrintNonterminal(nonterminal);
                std::cerr <<  " -> ";
                debugPrintProduction(production);
                std::cerr << std::endl;
            }
        } 
    }
}

bool Grammar::isChomsky() const {
    for (int nonterminal = 0; 
            nonterminal < getNonterminalsCount(); 
            ++nonterminal) {

        for (int production_index = 0; 
                production_index < getProductionsCount(nonterminal);
                ++production_index) {
            auto production = getProduction(nonterminal, production_index);
            if (production.size() == 2 && 
                    isNonterminal(production[0]) && 
                    isNonterminal(production[1])) {
                continue;
            } else if (production.size() == 1 &&
                    isTerminal(production[0])) {
                continue;
            } else if (production.size() == 1 && 
                    isSentinel(production[0]) &&
                    getGoal() == nonterminal) {
                continue;
            } else if (production.size() == 0) {
                continue;
            } else {
                std::cerr << "Bad production: ";
                debugPrintProduction(production);
                return false;
            }
        }
    }
    return true;
}

// EpsilonProducingProductionsRemover implementation

std::vector<bool> EpsilonProducingProductionsRemover::getEpsilonProducingNonterminals
            (Grammar *grammar, bool *induces_any) {
    std::vector<bool> does_produce_epsilon(grammar->getNonterminalsCount(), false);
    
    for (int nonterminal = 0; 
            nonterminal < grammar->getNonterminalsCount(); 
            ++nonterminal) {
        
        bool epsilon_producing_production = false;
        for (int production_index = 0; 
                production_index < grammar->getProductionsCount(nonterminal) && 
                !epsilon_producing_production;
                ++production_index) {

            auto production = grammar->getProduction(nonterminal, production_index);
            if (grammar->isEpsilonProduction(production)) {
                does_produce_epsilon.at(nonterminal) = true;
                *induces_any = true;
                epsilon_producing_production = true;
                continue;
            } 
        }
    }

    return does_produce_epsilon;
}

std::vector<bool> EpsilonProducingProductionsRemover::getEpsilonInducingNonterminals
            (Grammar *grammar, bool *induces_any) {
    std::vector<bool> does_induce_epsilon = getEpsilonProducingNonterminals(grammar, induces_any);
    if (!*induces_any) {
        return does_induce_epsilon;
    }

    bool answer_changed = false;
    do { 
        answer_changed = false;
        for (int nonterminal = 0; 
                nonterminal < grammar->getNonterminalsCount();
                ++nonterminal) {
            if (does_induce_epsilon.at(nonterminal)) {
                continue;
            }

            for (int production_index = 0; 
                    production_index < grammar->getProductionsCount(nonterminal);
                    ++production_index) {
                if (does_induce_epsilon.at(nonterminal)) {
                    continue;
                }

                auto production = grammar->getProduction(nonterminal, production_index);
                bool epsilon_inducing_production = true;
                for (auto symbol_iterator = production.begin();
                        symbol_iterator != production.end() && 
                        epsilon_inducing_production;
                        ++symbol_iterator) {

                    if (!grammar->isNonterminal(*symbol_iterator) || 
                        !does_induce_epsilon.at(*symbol_iterator)) {
                        epsilon_inducing_production = false;
                    }
                }
                if (epsilon_inducing_production) {
                    does_induce_epsilon.at(nonterminal) = true;
                    answer_changed = true;
                }
            }
        }
    } while (answer_changed);

    return does_induce_epsilon;
}

void EpsilonProducingProductionsRemover::addEpsilonRemovedProductions(Grammar *grammar,
        Grammar::Production head,
        std::vector<Grammar::Symbol>::iterator tail_begin, 
        std::vector<Grammar::Symbol>::iterator tail_end,
        Grammar::Symbol nonterminal,
        std::vector<bool> nonterminal_induces_epsilon) {
    for (auto iterator = tail_begin; iterator != tail_end; ++iterator) {
        auto symbol = *iterator;
        if (grammar->isNonterminal(symbol) && 
                nonterminal_induces_epsilon.at(symbol)) {
            addEpsilonRemovedProductions(grammar,
                    head, 
                    iterator + 1, 
                    tail_end, 
                    nonterminal, 
                    nonterminal_induces_epsilon);

            head.push_back(symbol);
            addEpsilonRemovedProductions(grammar,
                    head, 
                    iterator + 1, 
                    tail_end, 
                    nonterminal, 
                    nonterminal_induces_epsilon);
            return;
        } else {
            head.push_back(symbol);
        }
    }

    grammar->addProduction(nonterminal, head); 
}

void EpsilonProducingProductionsRemover::removeEpsilonProductions(Grammar *grammar) { 
    for (int nonterminal = 0; 
            nonterminal < grammar->getNonterminalsCount(); 
            ++nonterminal) {
        
        for (int production_index = 0; 
                production_index < grammar->getProductionsCount(nonterminal);
                ++production_index) {
            const Grammar::Production &production = grammar->getProduction
                    (nonterminal, production_index);
            if (grammar->isEpsilonProduction(production)) {
                grammar->removeProduction(nonterminal, production_index);
            } 
        } 
    }
}

void EpsilonProducingProductionsRemover::addEpsilonInducingGoal(Grammar *grammar) {
    auto new_goal = grammar->makeNonterminal();
    auto old_goal = grammar->getGoal();
    
    Grammar::Production epsilon_production = { Grammar::EPSILON };
    grammar->addProduction(new_goal, epsilon_production);

    Grammar::Production old_goal_production = { old_goal };
    grammar->addProduction(new_goal, old_goal_production);

    grammar->setGoal(new_goal);
}

void EpsilonProducingProductionsRemover::removeEpsilonProducingProductions(Grammar *grammar) {
    bool induces_any = false;
    auto nonterminal_induces_epsilon = getEpsilonInducingNonterminals(grammar, &induces_any);

    if (!induces_any) {
        return;
    }
    
    for (int nonterminal = 0; 
            nonterminal < grammar->getNonterminalsCount(); 
            ++nonterminal) {

        int productions_count = grammar->getProductionsCount(nonterminal);     
        for (int production_index = 0; 
                production_index < productions_count;
                ++production_index) {
            auto production = grammar->getProduction(nonterminal, production_index);
            grammar->removeProduction(nonterminal, production_index);
            addEpsilonRemovedProductions(grammar,
                    Grammar::Production(),
                    production.begin(), 
                    production.end(), 
                    nonterminal, 
                    nonterminal_induces_epsilon);
        } 
    }
        
    removeEpsilonProductions(grammar);

    if (nonterminal_induces_epsilon.at(grammar->getGoal())) {
        addEpsilonInducingGoal(grammar);
    }
}

// LongProductionsRemover implementation

void LongProductionsRemover::removeLongProductions(Grammar *grammar) {
    for (int nonterminal = 0; 
            nonterminal < grammar->getNonterminalsCount(); 
            ++nonterminal) {

        for (int production_index = 0; 
                production_index < grammar->getProductionsCount(nonterminal);
                ++production_index) {
            auto production = grammar->getProduction(nonterminal, production_index);

            if (production.size() > 2) {
                LongProductionsRemover::addShortProductions(grammar, nonterminal, production);
                grammar->removeProduction(nonterminal, production_index);
            }
        } 
    }
}

void LongProductionsRemover::addShortProductions(Grammar *grammar, 
        Grammar::Symbol nonterminal, 
        const Grammar::Production &production) {
    auto previous_nonterminal = nonterminal;    
    size_t symbol_index = 0;

    for (; symbol_index < production.size() - 2; ++symbol_index) {
        auto new_nonterminal = grammar->makeNonterminal();
        Grammar::Production new_production = { production.at(symbol_index), new_nonterminal };
        grammar->addProduction(previous_nonterminal, new_production);

        previous_nonterminal = new_nonterminal;
    }

    Grammar::Production last_production = 
            { production.at(symbol_index), production.at(symbol_index + 1) };
    grammar->addProduction(previous_nonterminal, last_production);
}

// ProductionNonterminalizer implementation

void ProductionNonterminalizer::nonterminalizeProductions(Grammar *grammar) {
    for (int nonterminal = 0; 
            nonterminal < grammar->getNonterminalsCount(); 
            ++nonterminal) {    
        for (int production_index = 0; 
                production_index < grammar->getProductionsCount(nonterminal);
                ++production_index) {
            auto production = grammar->getProduction(nonterminal, production_index);

            if (production.size() > 1) {
                for (auto &symbol: production) {
                    if (grammar->isTerminal(symbol)) {
                        auto new_terminal = grammar->makeNonterminal();
                        Grammar::Production new_production = { symbol };
                        grammar->addProduction(new_terminal, new_production);
                        symbol = new_terminal; 
                    }
                }
                grammar->changeProduction(nonterminal, production_index, std::move(production));
            }
        }
    }
}

// ChainProductionsRemover implementation

bool ChainProductionsRemover::isChainedProduction(Grammar *grammar, 
        const Grammar::Production &production) {
    return (production.size() == 1 && grammar->isNonterminal(production.at(0)));
}

void ChainProductionsRemover::removeChains(Grammar *grammar) {
    std::vector<ChainedProduction> chained_productions;
    auto chain_pairs = getChainedPairs(grammar, &chained_productions);

    for (auto pair: chain_pairs) {
        if (pair.parent == pair.child) {
            continue;
        }
        for (int production_index = 0;
            production_index < grammar->getProductionsCount(pair.child);
            ++production_index) {
            auto production = grammar->getProduction(pair.child, production_index);

            if (!isChainedProduction(grammar, production)) {
                grammar->addProduction(pair.parent, production);
            }
        }
    }

    removeChainProductions(grammar, chained_productions);
}


void ChainProductionsRemover::removeChainProductions(Grammar *grammar, 
            const std::vector<ChainedProduction> &chained_productions) {
    for (auto production: chained_productions) {
        grammar->removeProduction(production.nonterminal, production.production_index);
    } 
}

std::set<ChainProductionsRemover::ChainedPair> ChainProductionsRemover::getChainedPairs
            (Grammar *grammar, std::vector<ChainedProduction> *chained_productions) {
    std::set<ChainedPair> previous_pairs, result_pairs;
    for (int nonterminal = 0; 
            nonterminal < grammar->getNonterminalsCount(); 
            ++nonterminal) {
        previous_pairs.insert(ChainedPair(nonterminal, nonterminal));
        result_pairs.insert(ChainedPair(nonterminal, nonterminal));
    }

    bool answer_changed = false;
    do {
        answer_changed = false;
        std::set<ChainedPair> pairs;
        for (auto pair: previous_pairs) {
            for (int production_index = 0;
                production_index < grammar->getProductionsCount(pair.child);
                ++production_index) {

                auto production = grammar->getProduction(pair.child, production_index);
                if (isChainedProduction(grammar, production)) {
                    pairs.insert(ChainedPair(pair.parent, production.at(0)));
                    auto result = result_pairs.insert(ChainedPair(pair.parent, production.at(0)));
                    if (result.second) {
                        answer_changed = true;
                    }

                    chained_productions->emplace_back(pair.child, production_index);
                }
            }
        }
        
        previous_pairs = pairs;
    } while (answer_changed);

    return result_pairs;
}

// ShortestStringFinder implementation

void ShortestStringFinder::initShortestStrings() { 
    for (int nonterminal = 0; 
            nonterminal < grammar_.getNonterminalsCount(); 
            ++nonterminal) {
        for (int production_index = 0; 
                production_index < grammar_.getProductionsCount(nonterminal);
                ++production_index) {

            auto production = grammar_.getProduction(nonterminal, production_index);
            if (production.size() == 1 && grammar_.isTerminal(production.at(0))) {
                std::string new_string = {static_cast<char>(production.at(0))}; 

                if (shortest_strings_.at(nonterminal).at(1).empty()) {
                    shortest_strings_.at(nonterminal).at(1) = new_string;
                } else {
                    shortest_strings_.at(nonterminal).at(1) = std::min
                        (shortest_strings_.at(nonterminal).at(1), new_string);
                }
            }
        }
    }
}

std::string ShortestStringFinder::getComposedString(Grammar::Symbol nonterminal_left, 
        Grammar::Symbol nonterminal_right, 
        size_t left_part_size,
        size_t length) const {
    auto left_part = shortest_strings_.at(nonterminal_left ).at(left_part_size);  
    auto right_part = shortest_strings_.at(nonterminal_right).at(length - left_part_size + 1); 

    if (!left_part.empty() && !right_part.empty()) {
        return left_part + right_part;
    } else {
        return std::string();
    }
}

bool ShortestStringFinder::grammarProducesEpsilon() const {
    for (int production_index = 0; 
            production_index < grammar_.getProductionsCount(grammar_.getGoal());
            ++production_index) {
        const Grammar::Production &production = grammar_.getProduction
                (grammar_.getGoal(), production_index);
        if (grammar_.isEpsilonProduction(production)) {
            return true;
        }
    }
    return false;
}

std::string ShortestStringFinder::computeShortestStringForNonterminal
            (Grammar::Symbol nonterminal, size_t length) {
    std::string answer;

    for (int production_index = 0; 
            production_index < grammar_.getProductionsCount(nonterminal);
            ++production_index) {

        auto production = grammar_.getProduction(nonterminal, production_index);
        if (production.size() == 2 && 
                grammar_.isNonterminal(production.at(0)) && 
                grammar_.isNonterminal(production.at(1))) {
       
            for (int left_part_size = 1; left_part_size <= length; ++left_part_size) {
                auto composed_string = getComposedString(production.at(0), 
                        production.at(1), 
                        left_part_size, 
                        length);
                if (composed_string.empty()) {
                    continue;
                }

                if (answer.empty() || (!answer.empty() && answer > composed_string)) {
                    answer = composed_string;
                }
            }
        }
    }
    return answer;
}

void ShortestStringFinder::computeShortestStrings() {
    initShortestStrings();

    for (size_t length = 1; length < max_length_; ++length) {
        for (int nonterminal = 0; 
                nonterminal < grammar_.getNonterminalsCount(); 
                ++nonterminal) {    
            shortest_strings_.at(nonterminal).at(length + 1) = 
                computeShortestStringForNonterminal(nonterminal, length);
        }
    } 
}

std::string ShortestStringFinder::getShortestString() {
    if (grammarProducesEpsilon()) {
        return std::string("$");
    }

    initShortestStrings();
    computeShortestStrings();

    Grammar::Symbol goal = grammar_.getGoal();

    std::string answer = shortest_strings_.at(goal).at(1);
    for (size_t length = 2; length <= max_length_; ++length) {
        auto new_string = shortest_strings_.at(goal).at(length);
        if (new_string.empty()) {
            continue;
        }

        if (answer.empty() || (!answer.empty() && answer > new_string)) {
            answer = new_string;
        } 
    }
    return answer;
}
