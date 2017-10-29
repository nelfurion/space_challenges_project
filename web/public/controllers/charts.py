from django.http import JsonResponse
from public.models import Chart

def handle(request):
    charts = Chart.objects.all()
    charts = [chart.as_json() for chart in charts]

    print('charts:')
    print(len(charts))

    return JsonResponse({
        'charts': charts
    })