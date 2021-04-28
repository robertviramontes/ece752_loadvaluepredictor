# Robert Viramontes

from __future__ import print_function
from __future__ import absolute_import

import m5
from m5.defines import buildEnv
from m5.objects import *

from common import ObjectList

# Add the very basic options that work also in the case of the no ISA
# being used, and consequently no CPUs, but rather various types of
# testers and traffic generators.
def addLvpOptions(parser):
    # LCT params
    parser.add_option("--lct-entries", type="int", default=512)
    parser.add_option("--lct-ctr-bits", type="int", default=2)
    parser.add_option("--lct-invalidate-zero", default=False)

    # LVPT params
    parser.add_option("--lvpt-entries", type="int", default=1024)
    parser.add_option("--lvpt-hist-depth", type="int", default=1)

    # CVU params
    parser.add_option("--cvu-entries", type="int", default=8)
    parser.add_option("--cvu-replacement", type="int", default=1,
                        help = "1: FIFO, 2: LRU, 3: NLRU, 4: MRU, 5: NMRU")
