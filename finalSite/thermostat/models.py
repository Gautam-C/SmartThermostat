from django.db import models

import serial

# Create your models here.

class Temperature(models.Model):
    currnt_value = models.IntegerField;
    set_value = models.IntegerField;
    unit = models.CharField(max_length=1)

    def update_temps(self):
        # ser = serial.Serial('/dev/ttyUSB0')

        # line = ser.readline().strip().lower()

        line = 'sC40r38'.lower()

        self.set_value = line[(line.index('s') + 2):line.index('r')]
        self.current_value = int(line[line.index('r') + 1:])
        self.unit = line[line.index('s') + 1:line.index('s') + 2]