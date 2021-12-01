import serial.tools.list_ports
import librosa
import serial
import time
import os


class SerialAudioSpec(object):
    class SerialException(Exception):
        pass

    RESAMPLE_RATE = 9600

    __audio_data__ = []
    __sample_rate__ = 0
    __y_offset__ = 0
    __temp__ = []

    __device__ = serial.Serial

    def __init__(
        self,
        file: str,
        sample_rate: int = 0,
        sample_time: int = 1,
        serial_port: str = "",
        baud_rate: int = 115200,
    ):
        self.__audio__data__, self.__sample_rate__ = (
            librosa.load(os.path.abspath(file), sr=sample_rate)
            if sample_rate > 800
            else librosa.load(os.path.abspath(file))
        )

        self.__audio_data__ = librosa.to_mono(self.__audio__data__)
        self.__audio_data__ = librosa.resample(
            self.__audio__data__, self.__sample_rate__, self.RESAMPLE_RATE
        )

        self.__sample_rate__ = self.RESAMPLE_RATE

        ports = [
            i.device for i in serial.tools.list_ports.comports() if "SER" in i.hwid
        ]

        if len(ports) >= 1:
            ports = [ports[0]]
        else:
            raise self.SerialException("Could not find serial device")

        self.__device__ = serial.Serial(
            ports[0] if len(serial_port) < 3 else serial_port,
            baudrate=baud_rate,
            timeout=1,
        )

        if not self.__device__.isOpen():
            self.__device__.open()

        time.sleep(1.25)

    def send_serial_block(self, initial: str = "AD", block: list = []):
        if len(block) < 32:
            self.__device__.write(
                x := "".join(
                    ["@", (initial + "R").rjust(3, "_")]
                    + ["#" + str(i) for i in block]
                    + ["#&"]
                ).encode("utf-8")
            )
            print(x)
        else:
            els = len(block)
            ops = int(els / 32) if els % 32 == 0 else int(els / 32) + (els % 32)

            for i in range(ops):
                self.__device__.write(
                    x := "".join(
                        [
                            "@",
                            (
                                initial + "I"
                                if i == 0
                                else initial + "A"
                                if i + 1 < ops
                                else initial + "E"
                            ).rjust(3, "_"),
                        ]
                        + [
                            "#" + str(i)
                            for i in (block[:32] if len(block) > 32 else block)
                        ]
                        + ["#&"]
                    ).encode("utf-8")
                )
                block = block[32:] if len(block) > 32 else block
                print(x)


if __name__ == "__main__":
    dev = SerialAudioSpec("./assets/blindfold.mp3")
    dev.send_serial_block("AD", [i + 1 for i in range(24)])
