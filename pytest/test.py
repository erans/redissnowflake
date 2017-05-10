import unittest
from rmtest import ModuleTestCase
import os

module_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), '../redissnowflakemodule.so')

class SnowflakeTestCase(ModuleTestCase(module_path, module_args=('1','1'))):

    def testCmd(self):
        with self.redis() as r:
            self.assertIsNotNone(r.execute_command('snowflake.getid'))

if __name__ == '__main__':
    unittest.main()
