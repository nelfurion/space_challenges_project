from django.conf.urls import url
import public.views as views

urlpatterns = [
    url(r'^$', views.index, name='home')
]

