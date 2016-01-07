#
#
#
KBUILD_CFLAGS = $(CFLAGS) -mgeneral-regs-only
.c.o:
	$(CC) -c $(KBUILD_CFLAGS) $(INC) $(DEFS) $<

.S.o:
	$(AS) -c $(AFLAGS) $(INC) $(DEFS) $<


$(TARGET_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET_ELF) $(OBJS) $(LDLIBS)
	$(OBJCOPY) -O binary $(TARGET_ELF) $(TARGET_BIN)

clean:
	$(RM) -f $(TARGET) $(TARGET_BIN) $(TARGET_ELF) $(OBJS)
