#
#
#

.SUFFIXES: .cxx

.c.o:
	$(CC) -c $(CFLAGS) $(INC) $(DEFS) $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INC) $(DEFS) $<

.S.o:
	$(AS) -c $(AFLAGS) $(INC) $(DEFS) $<
	
$(TARGET_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET_ELF) $(OBJS) $(LDLIBS)
	$(OBJCOPY) -O binary $(TARGET_ELF) $(TARGET_BIN)

clean:
	$(RM) -f $(TARGET) $(TARGET_BIN) $(TARGET_ELF) $(OBJS)
