from django.urls import path

from . import views

app_name = 'thermostat'
urlpatterns = [
    path('', views.index, name='index'),
    path('setTemp/', views.set_temp, name='set_temp'),
    path('setOutTime/', views.set_out_time, name='set_out_time'),
    path('updateMonitors/', views.update_monitors, name='update_monitors'),
]