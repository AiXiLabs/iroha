/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_BLOCK_BUILDER_H
#define IROHA_BLOCK_BUILDER_H

#include <utils/datetime.hpp>
#include <vector>

// Document's requred size = 2^32 - 1
constexpr int MaxTxs = 2 * 1e7; // (1LL << 32) - 1;

namespace sumeragi {

using byte_array_t = std::vector<uint8_t>;

struct Signature {
  std::string publicKey;
  std::string signature;
};

enum class State {
  committed,
  uncommitted
};

struct Block {
  std::vector <byte_array_t> txs;
  std::vector <Signature> peer_sigs;
  uint64_t created; // timestamp
  State state;
};

class BlockBuilder {
public:
  BlockBuilder();
  BlockBuilder& setTxs(const std::vector<byte_array_t>& txs);
  BlockBuilder& setBlock(const Block& block);
  BlockBuilder& addSignature(const Signature& sig);
  Block build();
  Block buildCommit();

private:

  enum BuildStatus: uint8_t {
    initWithTxs    = 1 << 0,
    initWithBlock  = 1 << 1,
    withSignature  = 1 << 2,
    blockFromTxs   = initWithTxs,
    blockFromBlock = initWithBlock
                     | withSignature,
    commit         = initWithBlock,
  };

  int buildStatus_;

  // initWithTxs
  std::vector <std::vector<uint8_t>> txs_;

  // initWithBlock
  Block block_;
  std::vector <Signature> peer_sigs;
  State state_;
};

};

#endif //IROHA_BLOCK_BUILDER_H