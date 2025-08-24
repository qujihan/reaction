// #include "reaction/reaction.h"
// #include <benchmark/benchmark.h>
// #include <chrono>
// #include <numeric>
// #include <vector>

// using namespace reaction;

// static void BM_TreeDeepDependency_Reaction(benchmark::State &state) {
//     int index = 1;
//     auto base = var(index);
//     std::vector<Calc<int>> currentLevel;
//     currentLevel.emplace_back(calc([&]() { return base(); }));

//     for (int depth = 0; depth < state.range(0); depth++) {
//         std::vector<Calc<int>> nextLevel;

//         for (auto &parent : currentLevel) {
//             auto left = calc([=]() { return parent() + 1; });
//             auto right = calc([=]() { return parent() - 1; });

//             nextLevel.emplace_back(left);
//             nextLevel.emplace_back(right);
//         }

//         currentLevel = std::move(nextLevel);
//     }

//     auto sum = calc([&]() {
//         int total = 0;
//         for (auto &node : currentLevel) {
//             total += node();
//         }
//         return total;
//     });

//     for (auto _ : state) {
//         base.value(++index);
//     }
// }

// static void BM_WideDependency_Reaction(benchmark::State &state) {
//     std::vector<Var<int>> vars;

//     for (int i = 0; i < state.range(0); i++) {
//         vars.emplace_back(var(i));
//     }

//     auto finalNode = calc([&]() {
//         int sum = 0;
//         for (auto &node : vars) {
//             sum += node();
//         }
//         return sum;
//     });

//     for (auto _ : state) {
//         for (int i = 0; i < state.range(0); i++) {
//             vars[i].value(i + state.iterations());
//         }
//         benchmark::DoNotOptimize(finalNode.get());
//     }
// }

// static void BM_RepeatDependency(benchmark::State& state) {
//     using namespace reaction;
//     int index = 1;
//     auto a = var(index);

//     std::vector<Calc<int>> level1;
//     for (int i = 0; i < state.range(0); ++i) {
//         level1.emplace_back(calc([=]() {return a() + i; }));
//     }

//     std::vector<Calc<int>> level2;
//     for (int i = 0; i < state.range(0); ++i) {
//         level2.emplace_back(calc([=, &level1]() {
//             int result = a();
//             for (const auto &node : level1) {
//                 result += node();
//             }
//             return result;
//         }));
//     }

//     auto finalNode = calc([&]() {
//         int sum = 0;
//         for (int i = 0; i < state.range(0); ++i) {
//             sum += level2[i % level2.size()]();
//         }
//         return sum + a();
//     });

//     for (auto _ : state) {
//         a.value(++index);
//     }
// }

// static void BM_RepeatDependency_Batch(benchmark::State& state) {
//     using namespace reaction;
//     int index = 1;
//     auto a = var(index);

//     std::vector<Calc<int>> level1;
//     for (int i = 0; i < state.range(0); ++i) {
//         level1.emplace_back(calc([=]() {return a() + i; }));
//     }

//     std::vector<Calc<int>> level2;
//     for (int i = 0; i < state.range(0); ++i) {
//         level2.emplace_back(calc([=, &level1]() {
//             int result = a();
//             for (const auto &node : level1) {
//                 result += node();
//             }
//             return result;
//         }));
//     }

//     auto finalNode = calc([&]() {
//         int sum = 0;
//         for (int i = 0; i < state.range(0); ++i) {
//             sum += level2[i % level2.size()]();
//         }
//         return sum + a();
//     });

//     auto ba = batch([&](){a.value(++index);});

//     for (auto _ : state) {
//         ba.execute();
//     }
// }

// BENCHMARK(BM_TreeDeepDependency_Reaction)->Arg(10);
// BENCHMARK(BM_WideDependency_Reaction)->Arg(10000);
// BENCHMARK(BM_RepeatDependency)->Arg(500);
// BENCHMARK(BM_RepeatDependency_Batch)->Arg(500);

// BENCHMARK_MAIN();