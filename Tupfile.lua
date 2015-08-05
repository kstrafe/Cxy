tup.rule('$(NETHACK_ROOT)/temp/<objs>', 'g++ %<objs> -o %o', {'$(NETHACK_ROOT)/binaries/test.elf', '$(NETHACK_ROOT)/binaries/<test>'})
tup.rule({'$(NETHACK_ROOT)/binaries/<test>'}, './%<test>')
