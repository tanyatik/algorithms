#include <vector>
#include <stdexcept>
#include <iostream>

class Grammar {
public:
    static constexpr int EPSILON = '$';

    typedef int Symbol;
    typedef std::vector<Symbol> Production;

    static constexpr int ERROR_NONTERMINAL = -1;

    enum SymbolType {
        TERMINAL,
        NONTERMINAL,
        SENTINEL
    };

private:
    static constexpr char FIRST_TERMINAL = 'a';
    static constexpr char LAST_TERMINAL = 'z'; 
    static constexpr char FIRST_NONTERMINAL = 'A';
    static constexpr char LAST_NONTERMINAL = 'Z'; 
    static constexpr char NONTERMINAL_GOAL_LETTER = 'S';

    std::vector<std::vector<Production>> productions_table_;
    std::vector<SymbolType> symbol_types_;
    Symbol goal_;

    void setSymbolType(Symbol symbol, SymbolType type) {
        if (symbol_types_.size() <= symbol) {
            symbol_types_.resize(symbol + 1);
        }
        symbol_types_.at(symbol) = type;
    }
    
    Symbol makeNonterminal(char letter);
    void addNonterminal(Symbol nonterminal);


public:
    Grammar() :
        productions_table_(),
        goal_(ERROR_NONTERMINAL)
        {}

    Symbol getGoal() const {
        return goal_;
    }

    SymbolType getSymbolType(Symbol symbol) const {
        return symbol_types_.at(symbol);
    }

    size_t getNonterminalsCount() const;
    Symbol makeNonterminal(); 
    void removeNonterminal(Symbol symbol);

    void addProduction(Symbol symbol, const Production &production);
    void removeProduction(Symbol symbol, size_t production_index) {
        auto &productions = productions_table_.at(symbol);
        productions.erase(productions.begin() + production_index);
    }
    size_t getProductionsCount(Symbol symbol) const;
    const Production &getProduction(Symbol symbol, size_t production_index) const;
    bool isEpsilonProduction(const Production &production) const;

    void readProductionFromString(const std::string &string);
    void debugPrintNonterminal(Symbol symbol);
    void debugPrintProduction(const Production &production);
    void debugPrint();
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

Grammar::Symbol Grammar::makeNonterminal(char letter) {
    Symbol nonterminal = letter - FIRST_NONTERMINAL;
    if (nonterminal < 0) {
        throw std::runtime_error(std::string("Wrong nonterminal ") + 
                std::to_string(nonterminal));
    }
    addNonterminal(nonterminal);
    if (letter == NONTERMINAL_GOAL_LETTER) {
        goal_ = nonterminal;
    }

    return nonterminal;
}

void Grammar::removeNonterminal(Symbol symbol) {
    productions_table_.at(symbol).clear();
}

void Grammar::addProduction(Symbol symbol, const Production &production) {
    productions_table_.at(symbol).push_back(production);
}

void Grammar::readProductionFromString(const std::string &production_string) {
    auto symbol = production_string.begin();
    if (!(FIRST_NONTERMINAL <= *symbol && *symbol <= LAST_NONTERMINAL)) {
        throw std::runtime_error("Wrong symbol in production (should be nonterminal): " + 
                std::to_string(*symbol));
    }
    Symbol left_nonterminal = makeNonterminal(*symbol);
    ++symbol;

    if (*symbol++ != '-') {
        throw std::runtime_error("Wrong symbol in production: (should be -)" + 
                std::to_string(*symbol));
    } 
    if (*symbol++ != '>') {
        throw std::runtime_error("Wrong symbol in production: (should be >)" + 
                std::to_string(*symbol));
    } 

    Production production;
   
    for (; symbol != production_string.end() && *symbol != '\0'; ++symbol) {
        if (FIRST_TERMINAL <= *symbol && *symbol <= LAST_TERMINAL) {
            setSymbolType(*symbol, TERMINAL);
            production.push_back(*symbol); 
        } else if (EPSILON == *symbol) {
            setSymbolType(*symbol, SENTINEL);
            production.push_back(*symbol);
        } else if (FIRST_NONTERMINAL <= *symbol && *symbol <= LAST_NONTERMINAL) {
            Symbol nonterminal = makeNonterminal(*symbol);
            production.push_back(nonterminal);
        } else {
            throw std::runtime_error("Wrong *symbol in production: " + std::to_string(*symbol));
        }
    } 

    addProduction(left_nonterminal, production);
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

                    if(grammar->getSymbolType(*symbol_iterator) != Grammar::NONTERMINAL || 
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
    } while(answer_changed);

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
        if (grammar->getSymbolType(symbol) == Grammar::NONTERMINAL && 
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

Grammar readGrammar(std::istream &stream = std::cin) {
    Grammar grammar;

    size_t total_productions_count; 
    std::cin >> total_productions_count; 

    for (auto production_index = 0; 
            production_index < total_productions_count; 
            ++production_index) {
        std::string string;
        stream >> string;
        grammar.readProductionFromString(string);
    }

    if (grammar.getGoal() == grammar.ERROR_NONTERMINAL) {
        throw std::runtime_error("No goal in grammar\n");
    }   
 
    return grammar;
}

void Grammar::debugPrintNonterminal(Symbol symbol) {
    if (symbol >= 26) {
        std::cout << "A";
        std::cout << int(symbol - 26);
    } else {
        std::cout << (char) (symbol + FIRST_NONTERMINAL);
    }
}

void Grammar::debugPrintProduction(const Production &production) {
    for (auto symbol: production) {
        switch(getSymbolType(symbol)) {
            case NONTERMINAL:
                debugPrintNonterminal(symbol);
                break;
            case TERMINAL: case SENTINEL:
                std::cout << (char) symbol;
                break;
            default:
                break;
        }
        std::cout << " ";
    } 
}

void Grammar::debugPrint() {
    for (int nonterminal = 0; 
            nonterminal < getNonterminalsCount(); 
            ++nonterminal) {

        for (int production_index = 0; 
                production_index < getProductionsCount(nonterminal);
                ++production_index) {
            auto production = getProduction(nonterminal, production_index);

            if (!production.empty()) {
                debugPrintNonterminal(nonterminal);
                std::cout <<  " -> ";
                debugPrintProduction(production);
                std::cout << std::endl;
            }
        } 
    }
}

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

int main() {
    Grammar g = readGrammar();
    LongProductionsRemover::removeLongProductions(&g);
    EpsilonProducingProductionsRemover::removeEpsilonProducingProductions(&g);
    g.debugPrint();
    return 0;
}
