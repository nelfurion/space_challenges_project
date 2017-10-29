from django.conf.urls import url
import public.views as views
import public.controllers.index as index
import public.controllers.charts as charts

urlpatterns = [
    url(r'^$', index.handle, name='index'),
    url(r'^charts/$', charts.handle, name='index'),
]