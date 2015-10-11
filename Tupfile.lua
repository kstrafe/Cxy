tup.rule('$(ROOT)/temp/<objs>', 'g++ %<objs> -o %o', {'$(ROOT)/binaries/test.elf', '$(ROOT)/binaries/<test>'})
tup.rule({'$(ROOT)/binaries/<test>'}, './%<test>')
