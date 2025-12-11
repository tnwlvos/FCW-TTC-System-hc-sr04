CC = gcc
CFLAGS = -Wall -lwiringPi

TARGET = fcw

SRCS = main.c \
       logic/fcw_logic.c \
       logic/systemstate.c \
       sensors/ultrasonic.c \
       output/buzzer.c \
       output/lcd.c \
       ../OutputCtrl.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# 개별 .o rule
%.o: %.c
	$(CC) -c $< -o $@

# -----------------------------
#  개별 모듈 테스트 타깃
# -----------------------------

lcd: output/lcd.c ../OutputCtrl.c
	$(CC) -o lcd output/lcd.c ../OutputCtrl.c $(CFLAGS)

ultra: sensors/ultrasonic.c ../OutputCtrl.c
	$(CC) -o ultra sensors/ultrasonic.c ../OutputCtrl.c $(CFLAGS)

buzzer: output/buzzer.c ../OutputCtrl.c
	$(CC) -o buzzer output/buzzer.c ../OutputCtrl.c $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET) lcd ultra buzzer

re: clean all
