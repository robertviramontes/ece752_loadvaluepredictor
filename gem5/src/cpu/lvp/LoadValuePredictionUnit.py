# -*- coding: utf-8 -*-
# Robert Viramontes
# Created: March 25, 2021
# Based on the learning/part2/simpleobject 

from m5.params import *
from m5.SimObject import SimObject

class LoadClassificationTable(SimObject):
    type = 'LoadClassificationTable'
    cxx_header = "cpu/lvp/load_classification_table.hh"

    localPredictorSize = Param.Unsigned(512, "Size of local predictor")
    localCtrBits = Param.Unsigned(2, "Bits per counter")

class LoadValuePredictionTable(SimObject):
    type = 'LoadValuePredictionTable'
    cxx_header = "cpu/lvp/load_value_prediction_table.hh"

    entries = Param.Unsigned(1024, "Number of entries in the predicttion table")
    historyDepth = Param.Unsigned(1, "History depth")

# class ConstantVerificationUnit(SimObject):
 
class LoadValuePredictionUnit(SimObject):
    type = 'LoadValuePredictionUnit'
    cxx_header = "cpu/lvp/load_value_prediction_unit.hh"

    load_classification_table = Param.LoadClassificationTable("A load classification table")
    load_value_prediction_table = Param.LoadValuePredictionTable(LoadValuePredictionTable(), "A load value prediction table")
