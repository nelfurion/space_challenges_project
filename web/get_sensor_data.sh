# every 30 seconds query for new data

while true
do 

python manage.py get_sensor_data 
echo $(date -u) ": seeding assets..."

python manage.py clean_and_seed chart
echo $(date -u) "seeding charts..."

echo $(date -u) "data seed completed..."
echo "seeping for 1 minutes..."
sleep 60
done
