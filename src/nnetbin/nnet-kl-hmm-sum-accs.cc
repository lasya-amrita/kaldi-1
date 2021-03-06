// nnetbin/nnet-kl-hmm-sum-accs.cc

// Copyright 2013  Idiap Research Institute (Author: David Imseng)
//                 Karlsruhe Institute of Technology (Author: Ngoc Thang Vu)
//                 Brno University of Technology (Author: Karel Vesely)

// See ../../COPYING for clarification regarding multiple authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
// WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache 2 License for the specific language governing permissions and
// limitations under the License.

#include "nnet/nnet-nnet.h"
#include "nnet/nnet-kl-hmm.h"
#include "base/kaldi-common.h"
#include "util/common-utils.h"


int main(int argc, char *argv[]) {
  using namespace kaldi;
  using namespace kaldi::nnet1;

  try {
    typedef int32 int32;
    const char *usage =
      "Sum multiple accumulated stats files for KL-HMM training.\n"
      "Usage: nnet-kl-hmm-sum-accs [options] nnet-component stats-in1 .. stats-inN\n";

    bool binary = true;
    int32 n_kl_states = 0;
    int32 n_posterior_dim = 0;
    ParseOptions po(usage);
    po.Register("binary", &binary, "Write output in binary mode");
    po.Register("nkl-states", &n_kl_states, "Number of states in Kl-HMM");

    po.Register("posterior-dim", &n_posterior_dim,
        "Dimensionality of posterior features");

    po.Read(argc, argv);

    if (po.NumArgs() < 2) {
      po.PrintUsage();
      exit(1);
    }

    std::string nnet_component_filename = po.GetArg(1);
    KlHmm kl_hmm(n_posterior_dim, n_kl_states, NULL);

    int num_accs = po.NumArgs() - 1;
    for (int i = 2, max = po.NumArgs(); i <= max; i++) {
      std::string stats_in_filename = po.GetArg(i);
      Input ki(stats_in_filename);
      kl_hmm.AddStats(ki.Stream(), binary);
    }

    // Write out the accs
    {
      Output ko(nnet_component_filename, binary);
      kl_hmm.Write(ko.Stream(), binary);
    }

    KALDI_LOG << "Summed " << num_accs << " stats ";
    KALDI_LOG << "Written nnet component to " << nnet_component_filename;
  } catch(const std::exception &e) {
    std::cerr << e.what();
    return -1;
  }
}


