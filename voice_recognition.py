import re
import speech_recognition as sr

move_x_forward = r"(move )?(\d+) seconds forward"
turn_left = r"(turn)? left"
turn_right = r"(turn)? right"
move_forward = r"(move)? forward"
move_back = r"(move)? back"

# Initialize the recognizer
r = sr.Recognizer()

# Use the default microphone as the audio source
with sr.Microphone() as source:
    print("Listening...")

    # Adjust for ambient noise levels
    r.adjust_for_ambient_noise(source)

    while True:
        # Capture the audio input
        audio = r.listen(source)

        try:
            print("Recognizing...")

            # Use the Google Web Speech API for speech recognition in Portuguese
            text = r.recognize_google(audio) #, language="pt-BR")

            match = re.match(move_x_forward, text)
            if match:
                number = int(match.group(2))
                print("Number of meters:", number)
            elif re.match(move_forward, text):
                print("forward")
            elif re.match(move_back, text):
                print("back")
            elif re.match(turn_left, text):
                print("left")
            elif re.match(turn_right, text):
                print("right")
        except:
            continue
        # except sr.UnknownValueError:
        #     continue
        # except sr.RequestError as e:
        #     print("Não foi possível solicitar os resultados da API de reconhecimento de fala do Google; {0}".format(e))
