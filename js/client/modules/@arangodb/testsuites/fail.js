/* jshint strict: false, sub: true */
/* global print */
'use strict';

// //////////////////////////////////////////////////////////////////////////////
// / DISCLAIMER
// /
// / Copyright 2016 ArangoDB GmbH, Cologne, Germany
// / Copyright 2014 triagens GmbH, Cologne, Germany
// /
// / Licensed under the Apache License, Version 2.0 (the "License")
// / you may not use this file except in compliance with the License.
// / You may obtain a copy of the License at
// /
// /     http://www.apache.org/licenses/LICENSE-2.0
// /
// / Unless required by applicable law or agreed to in writing, software
// / distributed under the License is distributed on an "AS IS" BASIS,
// / WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// / See the License for the specific language governing permissions and
// / limitations under the License.
// /
// / Copyright holder is ArangoDB GmbH, Cologne, Germany
// /
// / @author Max Neunhoeffer
// //////////////////////////////////////////////////////////////////////////////

const functionsDocumentation = {
  'fail': 'this job will always produce a failed result'
};
const optionsDocumentation = [
];
function fail (options) {
  return {
    failSuite: {
      status: false,
      total: 1,
      message: 'this suite will always fail.',
      duration: 2,
      failed: 1,
      failTest: {
        status: false,
        total: 1,
        duration: 1,
        message: 'this testcase will always fail.'
      },
      failSuccessTest: {
        status: true,
        duration: 1,
        message: 'this testcase will always succeed, though its in the fail testsuite.'
      }
    },
    successSuite: {
      status: true,
      total: 1,
      message: 'this suite will always be successfull',
      duration: 1,
      failed: 0,
      success: {
        status: true,
        message: 'this testcase will always be successfull',
        duration: 1
      }
    }
  };
};

function setup (testFns, defaultFns, opts, fnDocs, optionsDoc) {
  testFns['fail'] = fail;
  for (var attrname in functionsDocumentation) { fnDocs[attrname] = functionsDocumentation[attrname]; }
  for (var i = 0; i < optionsDocumentation.length; i++) { optionsDoc.push(optionsDocumentation[i]); }
}

exports.setup = setup;
