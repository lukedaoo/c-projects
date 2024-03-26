MORSE_INPUT=$@

echo "MORSE: $MORSE_INPUT"

DOT_DELAY=0.2
DASH_DELAY=0.5
CHARACTER_GAP_DELAY=1
LOOP_DELAY=1.3
INTERVAL=2;

LED_LID_KERNEL="sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness"

delay() {
    sleep $1
}

led() {
    status=$1;
    if [[ "$status" == "on" ]]; then # short delay
         echo 1 | $LED_LID_KERNEL > /dev/null
    else 
         echo 0 | $LED_LID_KERNEL > /dev/null;
    fi
}

for ((j = 0; j <$INTERVAL ; j++)); do
    for (( i=0; i<${#MORSE_INPUT}; i++ )); do
        c="${MORSE_INPUT:$i:1}"
        if [ "$c" = "." ]; then # short 0.1
            led "off"
            delay $DOT_DELAY
            led "on"
        elif  [ "$c" = "-" ]; then # long delay
            led "off"
            delay $DASH_DELAY
            led "on"
        else 
            led "off"
            delay $CHARACTER_GAP_DELAY
            led "on"
        fi
    done
    delay $LOOP_DELAY
done
echo "DONE"
