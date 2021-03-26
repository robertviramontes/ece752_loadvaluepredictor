# -*- coding: utf-8 -*-
# Robert Viramontes
# Created: March 25, 2021

from m5.params import *
from m5.SimObject import SimObject

class LoadClassificationTable(SimObject):
    type = 'LoadClassificationTable'
    cxx_header = "cpu/lvp/lct.hh"

    localPredictorSize = Param.Unsigned(512, "Size of local predictor")
    localCtrBits = Param.Unsigned(2, "Bits per counter")

    goodbye_object = Param.GoodbyeObject("A goodbye object")

# class LoadValuePredictionTable(SimObject):

# class ConstantVerificationUnit(SimObject):
 
# class LoadValuePredictionUnit(SimObject):
    load_classification_table = Param.LoadClassificationTable("A load classification table")
