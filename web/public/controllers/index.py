from django.shortcuts import render

def handle(request):
    return render(request, 'public/index.html')