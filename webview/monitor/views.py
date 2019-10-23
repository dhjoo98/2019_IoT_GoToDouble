from django.http import JsonResponse
from django.shortcuts import render
from rest_framework.views import APIView
from monitor.models import Fposition,Sposition


class FpositionView(APIView):
    def get(self, request):
        if not Fposition.objects.count():
            return JsonResponse({'code': '-1'})
        position = Fposition.objects.last()
        angle = position.angle
        distance = position.distance
        time = position.time
        ret = JsonResponse({'code': '0', 'angle': float(angle), 'distance': float(distance), 'time': str(time)})
        ret['Access-Control-Allow-Origin'] = '*'
        return ret

    def post(self, request):
        angle = request.POST.get('angle')
        distance = request.POST.get('distance')
        if angle and distance:
            position = Fposition.objects.create(angle=angle,distance=distance)
            position.save()
            return JsonResponse({'code': '0'})
        else:
            return JsonResponse({'code': '-1'})


class SpositionView(APIView):
    def get(self, request):
        if not Sposition.objects.count():
            return JsonResponse({'code': '-1'})
        position = Sposition.objects.last()
        angle = position.angle
        distance = position.distance
        time = position.time
        ret = JsonResponse({'code': '0', 'angle': float(angle), 'distance': float(distance), 'time': str(time)})
        ret['Access-Control-Allow-Origin'] = '*'
        return ret

    def post(self, request):
        angle = request.POST.get('angle')
        distance = request.POST.get('distance')
        if angle and distance:
            position = Sposition.objects.create(angle=angle,distance=distance)
            position.save()
            return JsonResponse({'code': '0'})
        else:
            return JsonResponse({'code': '-1'})


def index(request):
    return render(request,'monitor/index.html')
