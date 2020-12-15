from django.shortcuts import get_object_or_404, redirect, render
from django.http import HttpResponse, JsonResponse
from django.urls import reverse
from django.conf.urls import url
from django.contrib import messages

from .models import Temperature

import serial

therm1 = Temperature()

# Create your views here.
def index(request):

    therm1.update_temps()

    if therm1.unit == 'f':
        set_temp = str(therm1.set_value) + 'ºF'
        current_temp = str(therm1.current_value) + 'ºF'
    elif therm1.unit == 'c':
        set_temp = str(therm1.set_value) + 'ºC'
        current_temp = str(therm1.current_value) + 'ºC'

    context = {
        "set_temp": set_temp,
        "current_temp": current_temp
    }
    return render(request, 'thermostat/index.html', context)
    

def set_temp(request):
    
    temp = request.POST.get('value', None)
    unit = request.POST.get('unit', None)

    if temp == '':
        messages.error(request, "No input was given")
        return redirect(reverse('thermostat:index'))

    ard_cmd = unit + temp

    '''
    Do not uncomment until deployed
    ser = serial.Serial('/dev/ttyUSB0')

    ser.write(ard_cmd)
    '''
    return redirect(reverse('thermostat:index'))

def set_out_time(request):

    start_out_time = request.POST.get('startOutTime', None)
    end_out_time = request.POST.get('endOutTime', None)
    out_temp = request.POST.get('outTemp', None)
    unit = request.POST.get('outUnit', None)

    if (start_out_time == '' or end_out_time == '') or out_temp == '':

        messages.error(request, "No input was given")

        return redirect(reverse('thermostat:index'))

    start_out_time = start_out_time[:start_out_time.index(':')] + start_out_time[start_out_time.index(':') + 1:]
    
    end_out_time = end_out_time[:end_out_time.index(':')] + end_out_time[end_out_time.index(':') + 1:]

    ard_cmd = 'o' + unit + str(out_temp) + str(start_out_time) + str(end_out_time)
    '''
    Do not uncomment until deployed
    ser = serial.Serial('/dev/ttyUSB0')

    ser.write(ard_cmd)
    '''

    return redirect(reverse('thermostat:index'))

def update_monitors(request):
    therm1.update_temps()

    if therm1.unit == 'f':
        set_temp = str(therm1.set_value) + 'ºF'
        current_temp = str(therm1.current_value) + 'ºF'
    elif therm1.unit == 'c':
        set_temp = str(therm1.set_value) + 'ºC'
        current_temp = str(therm1.current_value) + 'ºC'

    context = {
        "set_temp": set_temp,
        "current_temp": current_temp
    }
    
    test = {
        "set_temp": "50ºC",
        "current_temp": "48ºC"
    }
    
    return JsonResponse(test)