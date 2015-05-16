#
#
#
.c.o:
	$(CC) -c $(CFLAGS) $(INC) $(DEFS) -v $<

.S.o:
	$(AS) -c $(AFLAGS) $(INC) $(DEFS) -v $<
	
$(TARGET_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET_ELF) $(OBJS) $(LDLIBS)
	$(OBJCOPY) -O binary $(TARGET_ELF) $(TARGET_BIN)

clean:
	$(RM) -f $(TARGET) $(TARGET_BIN) $(TARGET_ELF) $(OBJS)
