/*
 * Inspired by Gjs
 *
 * Copyright (C) 2008 litl, LLC
 * Copyright (C) 2010 Intel Corporation
 *
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 */

var daxVerbose = 0;
var daxSuccess = 2;
var daxFailures = [];

function daxSetVerbose(verbose) {
    daxVerbose = verbose;
}

function daxTestsLog(message) {
    if (daxVerbose)
        print(message);
}

function daxTestsRun(obj, setUp, tearDown) {
  if (!obj)
    return 0;

  daxSuccess = 1;

  if (!setUp)
    setUp = obj.setUp;
  if (!tearDown)
    tearDown = obj.tearDown;

  for (propName in obj) {
    if (!propName.match(/^test\w+/))
      continue;

    var testFunction = obj[propName];
    if (typeof(testFunction) != 'function')
      continue;

    daxTestsLog("running test " + propName);

    setUp();
    try {
      testFunction();
    } catch (e) {
      var result = null;
      if (typeof(e.isJsUnitFailure) != 'undefined' && e.isJsUnitFailure) {
        result = '';
        if (e.comment != null)
          result += ('"' + e.comment + '"\n');

        result += e.jsUnitMessage;

        if (e.stackTrace)
          result += '\n\nStack trace follows:\n' + e.stackTrace;

        // assertion failure, kind of expected so just log it and flag the
        // whole test as failed
        printerr(result);
        daxSuccess = 0;
        daxFailures.push(propName);
      }
      else {
        // unexpected error, let the shell handle it
        throw e;
      }
    }
    tearDown();
  }

  if (daxFailures.length > 0) {
    printerr(daxFailures.length + " tests failed in this file");
    printerr("Failures were: " + daxFailures.join(", "));
  }

  return daxSuccess;
}
