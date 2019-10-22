from django.urls import path
from monitor import views

urlpatterns = [
    path('fposition', views.FpositionView.as_view()),
    path('sposition', views.SpositionView.as_view()),
    path('show', views.index),
]
