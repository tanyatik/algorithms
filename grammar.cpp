#include <vector>
#include <stdexcept>
#include <iostream>
#include <set>
#include <string>
#include <assert.h>
#include <algorithm>

// Represents context-free grammar
class Grammar {
public:
    static constexpr size_t TERMINAL_START_INDEX = 1;
    static constexpr size_t EPSILON_INDEX = 0;

    typedef int Symbol;
    typedef std::vector<Symbol> Production;

    static constexpr int ERROR_NONTERMINAL = -1;

private:
    // For every symbol that can appear on the left side, stores all possible productions
    std::vector<std::vector<Production>> productions_table_;
    size_t alphabet_size_;
    Symbol goal_;

public:
    explicit Grammar(int alphabet_size) :
        productions_table_(),
        alphabet_size_(alphabet_size),
        goal_(ERROR_NONTERMINAL)
        {}

    bool hasGoal() const;
    void setGoal(Symbol goal);
    Symbol getGoal() const;

    bool isTerminal(Symbol symbol) const;
    bool isNonterminal(Symbol symbol) const;
    bool isEpsilon(Symbol symbol) const;

    size_t getFirstNonterminalIndex() const;
    size_t getLastNonterminalIndex() const;

    // Returns symbol representing new terminal in grammar
    Symbol createNonterminal(); 
    // Returns symbol by its number in source alphabet
    Symbol getTerminalSymbol(int terminal_alphabet_number) const; 
    // Returns number of symol in source alphabet by its symbol in grammar
    int getTerminalAlphabetNumber(Symbol terminal_symbol) const; 
    Symbol getEpsilonSymbol() const; 

    size_t getProductionsCount(Symbol symbol) const;
    const Production &getProduction(Symbol symbol, size_t production_index) const;
    void removeProduction(Symbol symbol, size_t production_index);
    void setProduction(Symbol symbol, size_t production_index, Production production);
    void addProduction(Symbol symbol, const Production &production);

    bool isEpsilonProduction(const Production &production) const;
    bool doesProduceEpsilon() const;
};

class GrammarReader {
private:
    static constexpr char FIRST_TERMINAL = 'a';
    static constexpr char LAST_TERMINAL = 'z'; 
    static constexpr char FIRST_NONTERMINAL = 'A';
    static constexpr char LAST_NONTERMINAL = 'Z'; 
    static constexpr char NONTERMINAL_GOAL_LETTER = 'S';
    static constexpr char EPSILON = '$';

    enum { NOT_FILLED = -1 };

    // Map for symbols of alphabet to symbols of grammar
    std::vector<Grammar::Symbol> nonterminal_map_;

    // Returns symbol of grammar for specified symbol of alphabet
    // If there is no such symbol in grammar yet, creates one
    Grammar::Symbol getNonterminalSymbol(Grammar *grammar, char letter);
    Grammar::Symbol getTerminal(const Grammar *grammar, char symbol);
    Grammar::Symbol getEpsilon(const Grammar *grammar);

    // Reads a single production in form A->bD
    void readProductionFromString(Grammar *grammar, const std::string &production_string);

public:
    GrammarReader() :
        nonterminal_map_(LAST_NONTERMINAL + 1, NOT_FILLED)
        {}

    Grammar readGrammar(std::istream &stream = std::cin);
    void writeGrammarProduction(const Grammar &grammar,
            const Grammar::Production &production, 
            std::ostream &stream = std::cout);
};

class ProductionNonterminalizer {
public:
    // Performs equivalent transformation of grammar
    // with no rules with terminals in right part and length > 1
    static void nonterminalizeProductions(Grammar *grammar);
};

class LongProductionsRemover {
private:
    static void addShortProductions(Grammar *grammar, 
            Grammar::Symbol nonterminal, 
            const Grammar::Production &production);

public:
    // Performs equivalent transformation of grammar
    // with no rules of length > 2
    static void removeLongProductions(Grammar *grammar);
};

class EpsilonProductionProductionsRemover {
private:
    static void addEpsilonInducingGoal(Grammar *grammar);
    // Returns nonterminals A | there is rule A->eps in grammar
    static std::vector<bool> getEpsilonProductionNonterminals(Grammar *grammar, bool *induces_any);
    // Returns nonterminals A | there exists a chain A =>* eps 
    static std::vector<bool> getEpsilonInducingNonterminals(Grammar *grammar, bool *induces_any);
    // Recursive procedure which adds for rule A->A1 B1 A2 B2 ... An Bn,
    // (where A1 ... An are some chains of terminals or nonterminals,
    //  and B1 ... Bn are epsilon inducing terminals)
    // all possible rules with one of Bi removed
    static void addEpsilonRemovedProductions(Grammar *grammar,
            Grammar::Production head,
            std::vector<Grammar::Symbol>::iterator tail_begin, 
            std::vector<Grammar::Symbol>::iterator tail_end,
            Grammar::Symbol nonterminal,
            std::vector<bool> nonterminal_induces_epsilon);
    // Removes all productions like A->eps
    static void removeEpsilonProductions(Grammar *grammar);
    
public:
    // Performs equivalent transformation of grammar
    // with no rules like A->eps
    static void removeEpsilonProductionProductions(Grammar *grammar);
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
    // Performs equivalent transformation of grammar with no rules A->B
    static void removeChains(Grammar *grammar);
};

class ChomskyConverter {
public:
    static void convertToChomskyNormalForm(Grammar *grammar);
};

class ShortestProductionFinder {
private:
    const Grammar &grammar_;
    std::vector<std::vector<Grammar::Production>> shortest_productions_;
    size_t max_length_;
   
    void initShortestProductions(); 
    void computeShortestProductions(); 
    Grammar::Production computeShortestProductionForNonterminal
            (Grammar::Symbol nonterminal, size_t length);
    Grammar::Production getComposedProduction(Grammar::Symbol nonterminal_left, 
            Grammar::Symbol nonterminal_right, 
            size_t left_part_size,
            size_t length) const;

public:
    ShortestProductionFinder(const Grammar &grammar, size_t max_length) :
        grammar_(grammar),
        shortest_productions_
            (grammar.getLastNonterminalIndex() + 1, 
             std::vector<Grammar::Production>(max_length + 1)),
        max_length_(max_length)
        {}

    // Finds shortest possible production of grammar of length <= max_length_
    Grammar::Production getShortestProduction();
};

int readMaximumLength(std::istream &stream = std::cin);

int main() {
    GrammarReader reader;

    Grammar grammar = reader.readGrammar();
    int maximum_length = readMaximumLength();

    if (!grammar.hasGoal()) {
        reader.writeGrammarProduction(grammar, Grammar::Production());
        return 0;
    }

    ChomskyConverter::convertToChomskyNormalForm(&grammar);

    ShortestProductionFinder finder(grammar, maximum_length);
    auto shortest_production = finder.getShortestProduction();  

    reader.writeGrammarProduction(grammar, shortest_production);

    return 0;
}

// Grammar implementation

bool Grammar::hasGoal() const {
    return goal_ != ERROR_NONTERMINAL;
}

void Grammar::setGoal(Symbol goal) {
    goal_ = goal;
}

Grammar::Symbol Grammar::getGoal() const {
    return goal_;
}

bool Grammar::isTerminal(Symbol symbol) const {
    return symbol >= TERMINAL_START_INDEX && symbol < TERMINAL_START_INDEX + alphabet_size_;
} 

bool Grammar::isNonterminal(Symbol symbol) const {
    return symbol >= TERMINAL_START_INDEX + alphabet_size_;
}

bool Grammar::isEpsilon(Symbol symbol) const {
    return symbol == EPSILON_INDEX;
}

size_t Grammar::getFirstNonterminalIndex() const {
    return 0;
}

size_t Grammar::getLastNonterminalIndex() const {
    return productions_table_.size();
}

Grammar::Symbol Grammar::getTerminalSymbol(int terminal_alphabet_number) const {
    return terminal_alphabet_number + 1;
}

int Grammar::getTerminalAlphabetNumber(Grammar::Symbol symbol) const {
    return symbol - 1;
}

Grammar::Symbol Grammar::getEpsilonSymbol() const {
    Symbol index = 0;
    return index;
}

Grammar::Symbol Grammar::createNonterminal() {
    Symbol new_nonterminal = std::max(productions_table_.size(), 
            static_cast<size_t>(alphabet_size_ + 1));
    if (productions_table_.size() <= new_nonterminal) {
        productions_table_.resize(new_nonterminal + 1);
    }
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

void Grammar::removeProduction(Symbol symbol, size_t production_index) {
    auto &productions = productions_table_.at(symbol);
    productions.at(production_index).clear();
}

void Grammar::setProduction(Symbol symbol, size_t production_index, Production production) {
    productions_table_.at(symbol).at(production_index) = production;
}

bool Grammar::isEpsilonProduction(const Grammar::Production &production) const {
    bool result = production.size() == 1 && isEpsilon(production.at(0));
    return result;
}

bool Grammar::doesProduceEpsilon() const {
    for (int production_index = 0; 
            production_index < getProductionsCount(getGoal());
            ++production_index) {
        const Production &production = getProduction
                (getGoal(), production_index);
        if (isEpsilonProduction(production)) {
            return true;
        }
    }
    return false;
}

// GrammarReader implementation

Grammar::Symbol GrammarReader::getNonterminalSymbol(Grammar *grammar, char symbol) {
    size_t symbol_index = symbol;
    
    if (nonterminal_map_.at(symbol_index) == NOT_FILLED) {
        nonterminal_map_.at(symbol_index) = grammar->createNonterminal();
        if (symbol == NONTERMINAL_GOAL_LETTER) {
            grammar->setGoal(nonterminal_map_.at(symbol_index));
        }
    } 

    return nonterminal_map_.at(symbol_index);
}

void GrammarReader::readProductionFromString(Grammar *grammar, 
        const std::string &production_string) {
    auto symbol = production_string.begin();

    auto left_nonterminal = getNonterminalSymbol(grammar, *symbol);
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
            production.push_back(grammar->getTerminalSymbol(*symbol - FIRST_TERMINAL)); 
        } else if (EPSILON == *symbol) {
            production.push_back(grammar->getEpsilonSymbol());
        } else if (FIRST_NONTERMINAL <= *symbol && *symbol <= LAST_NONTERMINAL) {
            production.push_back(getNonterminalSymbol(grammar, *symbol));
        } else {
            throw std::runtime_error("Wrong *symbol in production: " + std::to_string(*symbol));
        }
    } 

    grammar->addProduction(left_nonterminal, production);
}

Grammar GrammarReader::readGrammar(std::istream &stream) {
    Grammar grammar(LAST_TERMINAL - FIRST_TERMINAL + 1);

    size_t total_productions_count; 
    stream >> total_productions_count; 

    for (auto production_index = 0; 
            production_index < total_productions_count; 
            ++production_index) {
        std::string string;
        stream >> string;

        readProductionFromString(&grammar, string);
    }

    return grammar;
}

void GrammarReader::writeGrammarProduction(const Grammar &grammar,
        const Grammar::Production &production, 
        std::ostream &stream) {
    if (production.empty()) {
        stream << "IMPOSSIBLE" << std::endl;
    } else {
        for (auto symbol: production) {
            if (grammar.getEpsilonSymbol() == symbol) {
                stream << EPSILON;
            } else {
                int alphabet_symbol = grammar.getTerminalAlphabetNumber(symbol) + FIRST_TERMINAL;
                stream << static_cast<char>(alphabet_symbol); 
            }
        }
        stream << std::endl;
    }
}

int readMaximumLength(std::istream &stream) {
    int max_length;
    stream >> max_length;
    return max_length;
}

// EpsilonProductionProductionsRemover implementation

std::vector<bool> EpsilonProductionProductionsRemover::getEpsilonProductionNonterminals
            (Grammar *grammar, bool *induces_any) {
    std::vector<bool> does_produce_epsilon(grammar->getLastNonterminalIndex(), false);
    
    for (int nonterminal = grammar->getFirstNonterminalIndex(); 
            nonterminal < grammar->getLastNonterminalIndex(); 
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

std::vector<bool> EpsilonProductionProductionsRemover::getEpsilonInducingNonterminals
            (Grammar *grammar, bool *induces_any) {
    std::vector<bool> does_induce_epsilon = getEpsilonProductionNonterminals(grammar, induces_any);
    if (!*induces_any) {
        return does_induce_epsilon;
    }

    bool answer_changed = false;
    do { 
        answer_changed = false;
        for (int nonterminal = grammar->getFirstNonterminalIndex(); 
                nonterminal < grammar->getLastNonterminalIndex();
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

                if (production.size() == 0) {
                    continue;
                }

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

void EpsilonProductionProductionsRemover::addEpsilonRemovedProductions(Grammar *grammar,
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

void EpsilonProductionProductionsRemover::removeEpsilonProductions(Grammar *grammar) { 
    for (int nonterminal = grammar->getFirstNonterminalIndex(); 
            nonterminal < grammar->getLastNonterminalIndex(); 
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

void EpsilonProductionProductionsRemover::addEpsilonInducingGoal(Grammar *grammar) {
    auto new_goal = grammar->createNonterminal();
    auto old_goal = grammar->getGoal();
    
    Grammar::Production epsilon_production = { 0 };
    grammar->addProduction(new_goal, epsilon_production);

    Grammar::Production old_goal_production = { old_goal };
    grammar->addProduction(new_goal, old_goal_production);

    grammar->setGoal(new_goal);
}

void EpsilonProductionProductionsRemover::removeEpsilonProductionProductions(Grammar *grammar) {
    bool induces_any = false;
    auto nonterminal_induces_epsilon = getEpsilonInducingNonterminals(grammar, &induces_any);

    if (!induces_any) {
        return;
    }
    
    for (int nonterminal = grammar->getFirstNonterminalIndex(); 
            nonterminal < grammar->getLastNonterminalIndex(); 
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
    for (int nonterminal = grammar->getFirstNonterminalIndex(); 
            nonterminal < grammar->getLastNonterminalIndex(); 
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
        auto new_nonterminal = grammar->createNonterminal();
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
    for (int nonterminal = grammar->getFirstNonterminalIndex(); 
            nonterminal < grammar->getLastNonterminalIndex(); 
            ++nonterminal) {    
        for (int production_index = 0; 
                production_index < grammar->getProductionsCount(nonterminal);
                ++production_index) {
            auto production = grammar->getProduction(nonterminal, production_index);

            if (production.size() > 1) {
                for (auto &symbol: production) {
                    if (grammar->isTerminal(symbol)) {
                        auto new_terminal = grammar->createNonterminal();
                        Grammar::Production new_production = { symbol };
                        grammar->addProduction(new_terminal, new_production);
                        symbol = new_terminal; 
                    }
                }
                grammar->setProduction(nonterminal, production_index, production);
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
    auto old_grammar = Grammar(*grammar);

    for (auto pair: chain_pairs) {

        if (pair.parent == pair.child) {
            continue;
        }

        for (int production_index = 0;
                production_index < old_grammar.getProductionsCount(pair.child);
                ++production_index) {
            auto production = old_grammar.getProduction(pair.child, production_index);

            if (!isChainedProduction(&old_grammar, production)) {
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
    for (int nonterminal = grammar->getFirstNonterminalIndex(); 
            nonterminal < grammar->getLastNonterminalIndex(); 
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

// ChomskyConverter implementation

void ChomskyConverter::convertToChomskyNormalForm(Grammar *grammar) {
    ProductionNonterminalizer::nonterminalizeProductions(grammar);
    LongProductionsRemover::removeLongProductions(grammar);
    EpsilonProductionProductionsRemover::removeEpsilonProductionProductions(grammar);
    ChainProductionsRemover::removeChains(grammar);
}

// ShortestProductionFinder implementation

void ShortestProductionFinder::initShortestProductions() { 
    for (int nonterminal = grammar_.getFirstNonterminalIndex(); 
            nonterminal < grammar_.getLastNonterminalIndex(); 
            ++nonterminal) {
        for (int production_index = 0; 
                production_index < grammar_.getProductionsCount(nonterminal);
                ++production_index) {

            auto production = grammar_.getProduction(nonterminal, production_index);
            if (production.size() == 1 && grammar_.isTerminal(production.at(0))) {
                Grammar::Production new_production = {static_cast<char>(production.at(0))}; 

                if (shortest_productions_.at(nonterminal).at(1).empty()) {
                    shortest_productions_.at(nonterminal).at(1) = new_production;
                } else {
                    shortest_productions_.at(nonterminal).at(1) = std::min
                        (shortest_productions_.at(nonterminal).at(1), new_production);
                }
            }
        }
    }
}

Grammar::Production ShortestProductionFinder::getComposedProduction
        (Grammar::Symbol nonterminal_left, 
        Grammar::Symbol nonterminal_right, 
        size_t left_part_size,
        size_t length) const {
    auto left_part = shortest_productions_.at(nonterminal_left ).at(left_part_size);  
    auto right_part = shortest_productions_.at(nonterminal_right).at(length - left_part_size + 1); 

    if (!left_part.empty() && !right_part.empty()) {
        left_part.insert(left_part.end(), right_part.begin(), right_part.end());
        return left_part;
    } else {
        return Grammar::Production();
    }
}

Grammar::Production ShortestProductionFinder::computeShortestProductionForNonterminal
            (Grammar::Symbol nonterminal, size_t length) {
    Grammar::Production answer;

    if (grammar_.doesProduceEpsilon()) {
        return Grammar::Production({ grammar_.getEpsilonSymbol() });
    }

    for (int production_index = 0; 
            production_index < grammar_.getProductionsCount(nonterminal);
            ++production_index) {

        auto production = grammar_.getProduction(nonterminal, production_index);
        if (production.size() == 2 && 
                grammar_.isNonterminal(production.at(0)) && 
                grammar_.isNonterminal(production.at(1))) {
       
            for (int left_part_size = 1; left_part_size <= length; ++left_part_size) {
                auto composed_production = getComposedProduction(production.at(0), 
                        production.at(1), 
                        left_part_size, 
                        length);
                if (composed_production.empty()) {
                    continue;
                }

                if (answer.empty() || (!answer.empty() && answer > composed_production)) {
                    answer = composed_production;
                }
            }
        }
    }
    return answer;
}

void ShortestProductionFinder::computeShortestProductions() {
    initShortestProductions();

    for (size_t length = 1; length < max_length_; ++length) {
        for (int nonterminal = grammar_.getFirstNonterminalIndex(); 
                nonterminal < grammar_.getLastNonterminalIndex(); 
                ++nonterminal) {    
            shortest_productions_.at(nonterminal).at(length + 1) = 
                computeShortestProductionForNonterminal(nonterminal, length);
        }
    } 
}

Grammar::Production ShortestProductionFinder::getShortestProduction() {
    if (grammar_.doesProduceEpsilon()) {
        return { grammar_.getEpsilonSymbol() };
    }

    initShortestProductions();
    computeShortestProductions();

    Grammar::Symbol goal = grammar_.getGoal();

    Grammar::Production answer = shortest_productions_.at(goal).at(1);
    for (size_t length = 2; length <= max_length_; ++length) {
        auto new_production = shortest_productions_.at(goal).at(length);
        if (new_production.empty()) {
            continue;
        }

        if (answer.empty() || (!answer.empty() && answer > new_production)) {
            answer = new_production;
        } 
    }
    return answer;
}
