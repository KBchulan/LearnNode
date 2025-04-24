/******************************************************************************
 *
 * @file       ParallelCompute.hpp
 * @brief      函数式编程实现并行计算
 *
 * @author     KBchulan
 * @date       2025/04/21
 * @history
 ******************************************************************************/

#ifndef PARALLEL_COMPUTE_HPP
#define PARALLEL_COMPUTE_HPP

#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class ParallelCompute final : public global::Singleton<ParallelCompute>{
public:
  static void enterFunc() noexcept;
};

} // namespace core

#define parallelCompute core::ParallelCompute::getInstance()

#endif // PARALLEL_COMPUTE_HPP