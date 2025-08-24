
#include <benchmark/benchmark.h>

#include <vector>

#include "rxcpp/rx.hpp"

using namespace rxcpp;
using namespace rxcpp::sources;
using namespace rxcpp::subjects;
using namespace rxcpp::operators;

static void BM_TreeDeepDependency_RxCpp(benchmark::State &state) {
    subject<int> base;

    struct TreeNode {
        observable<int> obs;
        std::vector<std::shared_ptr<TreeNode>> children;
    };

    auto root = std::make_shared<TreeNode>();
    root->obs = base.get_observable();
    std::vector<std::shared_ptr<TreeNode>> currentLevel{root};

    for (int depth = 0; depth < state.range(0); depth++) {
        std::vector<std::shared_ptr<TreeNode>> nextLevel;

        for (auto &parent : currentLevel) {
            auto left = std::make_shared<TreeNode>();
            left->obs = parent->obs.map([](int x) { return x + 1; });

            auto right = std::make_shared<TreeNode>();
            right->obs = parent->obs.map([](int x) { return x - 1; });

            parent->children = {left, right};
            nextLevel.push_back(left);
            nextLevel.push_back(right);
        }

        currentLevel = std::move(nextLevel);
    }

    int sum = 0;
    std::vector<rxcpp::subscription> subscriptions;

    for (auto &leaf : currentLevel) {
        subscriptions.push_back(
            leaf->obs.subscribe([&](int x) { sum += x; }));
    }

    for (auto _ : state) {
        sum = 0;
        base.get_subscriber().on_next(state.iterations());
        benchmark::DoNotOptimize(sum);
    }

    for (auto &sub : subscriptions) {
        sub.unsubscribe();
    }
}

static void BM_WideDependency_RxCpp(benchmark::State &state) {
    std::vector<std::shared_ptr<subject<int>>> vars;
    for (int i = 0; i < state.range(0); i++) {
        vars.push_back(std::make_shared<subject<int>>());
    }

    auto merged = rxcpp::observable<>::iterate(vars)
                      .flat_map([](std::shared_ptr<subject<int>> subj) {
                          return subj->get_observable();
                      })
                      .publish()
                      .ref_count();

    std::atomic<int> sum{0};
    auto sum_subscription = merged.scan(0, [](int acc, int x) {
                                      return acc + x;
                                  })
                                .subscribe([&](int current_sum) {
                                    sum.store(current_sum, std::memory_order_relaxed);
                                });

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            vars[i]->get_subscriber().on_next(i + state.iterations());
        }
        benchmark::DoNotOptimize(sum.load(std::memory_order_relaxed));
    }

    sum_subscription.unsubscribe();
}

BENCHMARK(BM_TreeDeepDependency_RxCpp)->Arg(10);
BENCHMARK(BM_WideDependency_RxCpp)->Arg(10000);

BENCHMARK_MAIN();