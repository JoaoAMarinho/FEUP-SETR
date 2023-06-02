import re
import speech_recognition as sr


class Voice(object):
    def __init__(self):
        self.move_x_forward = r"(move )?(\d+) seconds forward"
        self.turn_left = r"(turn )?left"
        self.turn_right = r"(turn )?right"
        self.move_forward = r"(move )?forward"
        self.move_back = r"(move )?back"
        self.stop = r"stop"

        # Initialize the recognizer
        self.r = sr.Recognizer()
        self.r.non_speaking_duration = 0.3
        self.r.pause_threshold = 0.3

    def voiceCommand(self):
        # Use the default microphone as the audio source
        with sr.Microphone() as source:
            # Adjust for ambient noise levels
            self.r.adjust_for_ambient_noise(source)
            print("Listening...")
            # Capture the audio input
            audio = self.r.listen(source)

            try:
                print("Recognizing...")

                # Use the Google Web Speech API for speech recognition in Portuguese
                response = self.r.recognize_google(audio, show_all=True) #, language="pt-BR")
                alternatives = response["alternative"]

                # Print each alternative
                for alternative in alternatives:
                    transcript = alternative["transcript"]
                    match = re.match(self.move_x_forward, transcript)
                    if match:
                        number = int(match.group(2))
                        print("Number of meters:", number)
                    elif re.match(self.move_forward, transcript):
                        print("forward")
                        return "forward"
                    elif re.match(self.move_back, transcript):
                        print("back")
                        return "back"
                    elif re.match(self.turn_left, transcript):
                        print("left")
                        return "left"
                    elif re.match(self.turn_right, transcript):
                        print("right")
                        return "right"
                    elif re.match(self.stop, transcript):
                        print("stop")
                        return "stop"
                    
            except:
                return "None"
                # except sr.UnknownValueError:
                #     continue
                # except sr.RequestError as e:
                #     print("Não foi possível solicitar os resultados da API de reconhecimento de fala do Google; {0}".format(e))

# Voice().voiceCommand()