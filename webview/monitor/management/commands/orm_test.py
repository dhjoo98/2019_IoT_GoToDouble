from django.core.management.base import BaseCommand, CommandError
from monitor.models import Fposition,Sposition

class Command(BaseCommand):
    def handle(self,*args,**kwargs):
        def f_save(angle,distance):
            position = Fposition.objects.create(angle=angle,distance=distance)
            position.save()

        def s_save(angle,distance):
            position = Sposition.objects.create(angle=angle,distance=distance)
            position.save()

        for i in range(100):
            f_save(float(i),float(i))
            s_save(float(i),float(i))
            print(Fposition.objects.count(), Sposition.objects.count())
            print(Fposition.objects.last(), Sposition.objects.last())
