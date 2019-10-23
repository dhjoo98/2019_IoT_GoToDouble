from django.http import JsonResponse
from django.shortcuts import render
from rest_framework.views import APIView
from monitor.models import Fposition,Sposition


class FpositionView(APIView):
    def get(self, request):
        if not Fposition.objects.count():
            return JsonResponse({'code': '-1'})
        position = Fposition.objects.order_by('time').first()
        angle = position.angle
        distance = position.distance
        time = position.time
        position.delete()
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
        position = Sposition.objects.order_by('time').first()
        angle = position.angle
        distance = position.distance
        time = position.time
        position.delete()
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
    if not Fposition.objects.count():
        fangle=fdistance=ftime=0
    else:
        fdata = Fposition.objects.order_by('time').first()
        fangle = fdata.angle
        fdistance = fdata.distance
        ftime = fdata.time
        fdata.delete()

    if not Sposition.objects.count():
        sangle=sdistance=stime=0
    else:
        sdata = Sposition.objects.order_by('time').first()
        sangle = sdata.angle
        sdistance = sdata.distance
        stime = sdata.time
        sdata.delete()
    return render(request,'monitor/index.html',{'fangle': fangle, 'fdistance': fdistance, 'ftime': ftime,
                'sangle': sangle, 'sdistance': sdistance, 'stime': stime})
