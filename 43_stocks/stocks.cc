#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <cfloat>

#include "stocks.h"


struct trade {
    double t;
    unsigned N;
    double p;
};

struct trades_log {
    double w;
    std::map<std::string, std::deque<struct trade>> T;
};

struct trades_log * new_log() {
    struct trades_log * l = new trades_log;
    l->w = 60;
    return l;
}		/* constructor */
void delete_log(struct trades_log * l) {
    delete l;
}	/* destructor */

int new_trade(struct trades_log * l, double time, const char * symbol, double price, unsigned int quantity) {

    // check validity of trade
    if (price <= 0 || quantity <= 0) return 0;
    for (auto & symbol_trades : l->T) {
        if (!symbol_trades.second.empty() && time < (symbol_trades.second.back()).t) return 0; // if time would be before a previous trade
    }

    // remove trades made vefore t - w
    for (auto & symbol_trades : l->T) {
        while (!symbol_trades.second.empty() && symbol_trades.second.front().t < time - l->w) {
            symbol_trades.second.pop_front();
        }
    }

    struct trade tr = {time, quantity, price};
    l->T[symbol].push_back(tr);
    return 1;
}
void clear(struct trades_log * l) {
    l->T.clear();
}

void set_time_window(struct trades_log * l, double w) {
    l->w = w;
    
    // get current time (maximum time of all trades)
    double t_now = FLT_MIN;
    for (auto & symbol_trades : l->T) {
        if (!symbol_trades.second.empty() && (symbol_trades.second.back()).t > t_now) t_now = (symbol_trades.second.back()).t;
    }

    // remove trades made vefore t_now - w
    for (auto & symbol_trades : l->T) {
        while (!symbol_trades.second.empty() && symbol_trades.second.front().t < t_now - l->w) {
            symbol_trades.second.pop_front();
        }
    }
}

unsigned int total_quantity(const struct trades_log * l) {
    unsigned tot = 0;
    for (auto & symbol_trades : l->T) {
        for (auto & trade : symbol_trades.second) {
            tot += trade.N;
        }
    }
    return tot;
}

double high_price(const struct trades_log * l) {
    if (l->T.empty()) return 0;
    double high = FLT_MIN;
    for (auto & symbol_trades : l->T) {
        for (auto & trade : symbol_trades.second) {
            if (trade.p > high) high = trade.p;
        }
    }
    return high;
}
double low_price(const struct trades_log * l) {
    if (l->T.empty()) return 0;
    double low = FLT_MAX;
    for (auto & symbol_trades : l->T) {
        for (auto & trade : symbol_trades.second) {
            if (trade.p < low) low = trade.p;
        }
    }
    return low;
}

double stock_high_price(const struct trades_log * l, const char * symbol) {
    if (l->T.find(symbol) == l->T.end()) return 0;
    double high = FLT_MIN;
    for (auto & trade : l->T.at(symbol)) {
        if (trade.p > high) high = trade.p;
    }
    return high;
}
double stock_low_price(const struct trades_log * l, const char * symbol) {
    if (l->T.find(symbol) == l->T.end()) return 0;
    double low = FLT_MAX;
    for (auto & trade : l->T.at(symbol)) {
        if (trade.p < low) low = trade.p;
    }
    return low;
}
double stock_avg_price(const struct trades_log * l, const char * symbol) {
    if (l->T.find(symbol) == l->T.end()) return 0;
    if (l->T.at(symbol).empty()) return 0;
    double price_sum = 0;
    unsigned price_num = 0;
    for (auto & trade : l->T.at(symbol)) {
        price_sum += trade.p;
        price_num++;
    }
    return price_sum / price_num;
}
