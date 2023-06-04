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
        self.kill = r"kill"

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
                    if re.match(self.move_forward, transcript):
                        return "forward"
                    elif re.match(self.move_back, transcript):
                        return "backward"
                    elif re.match(self.turn_left, transcript):
                        return "left"
                    elif re.match(self.turn_right, transcript):
                        return "right"
                    elif re.match(self.stop, transcript):
                        return "stop"
                    elif re.match(self.kill, transcript):
                        return "kill"

            except:
                return "None"
                # except sr.UnknownValueError:
                #     continue
                # except sr.RequestError as e:
                #     print("Não foi possível solicitar os resultados da API de reconhecimento de fala do Google; {0}".format(e))
