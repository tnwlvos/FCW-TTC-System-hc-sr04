CC = gcc
CFLAGS  = -Wall
LDFLAGS = -lwiringPi -pthread
TARGET = fcw

SRCS = main.c \
       logic/fcw_logic.c \
       logic/systemstate.c \
       sensors/ultrasonic.c \
       output/buzzer.c \
       output/lcd.c \
	   output/led.c \
	   output/io_select.c \
	   output/tm1637.c \
       ../OutputCtrl.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------
#  개별 모듈 테스트 타깃
# -----------------------------
lcd: output/lcd.c ../OutputCtrl.c
	$(CC) -o lcd output/lcd.c ../OutputCtrl.c $(LDFLAGS)

ultra: sensors/ultrasonic.c ../OutputCtrl.c
	$(CC) -o ultra sensors/ultrasonic.c ../OutputCtrl.c $(LDFLAGS)

buzzer: output/buzzer.c ../OutputCtrl.c
	$(CC) -o buzzer output/buzzer.c ../OutputCtrl.c $(LDFLAGS)

led: output/led.c ../OutputCtrl.c
	$(CC) -o led output/led.c ../OutputCtrl.c $(LDFLAGS)

tm1637: output/tm1637.c output/io_select.c ../OutputCtrl.c
	$(CC) -o tm1637 output/tm1637.c output/io_select.c ../OutputCtrl.c $(LDFLAGS)

io_select: output/io_select.c ../OutputCtrl.c
	$(CC) -o io_select output/io_select.c ../OutputCtrl.c $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET) lcd ultra buzzer led tm1637 io_select


re: clean all
