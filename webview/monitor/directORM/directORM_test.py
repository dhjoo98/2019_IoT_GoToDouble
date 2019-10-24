import os
import sys
from pathlib import Path
import django

currentPath = Path(os.getcwd())
sys.path.append(str(currentPath.parent.parent))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'webview.settings')
django.setup()

from monitor.models import Fposition, Sposition

def f_save(angle,distance):
    position = Fposition.objects.create(angle=angle,distance=distance)
    position.save()

def s_save(angle,distance):
    position = Sposition.objects.create(angle=angle,distance=distance)
    position.save()

def delete():
    Fposition.objects.all().delete()
    Sposition.objects.all().delete()
#test
for i in Fposition.objects.all():
    print(i.angle,i.distance)
delete()
