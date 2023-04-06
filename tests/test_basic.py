import pytest
import time

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`


async def test_first_time_users(service_client):
    response = await service_client.post(
        '/v1/hello',
        params={'name': 'userver'},
    )
    assert response.status == 200
    assert response.text == 'Hello, userver!\n'


async def test_db_updates(service_client):
    response = await service_client.post('/v1/hello', params={'name': 'World'})
    assert response.status == 200
    assert response.text == 'Hello, World!\n'

    response = await service_client.post('/v1/hello', params={'name': 'World'})
    assert response.status == 200
    assert response.text == 'Hi again, World!\n'

    response = await service_client.post('/v1/hello', params={'name': 'World'})
    assert response.status == 200
    assert response.text == 'Hi again, World!\n'


async def test_login(service_client):
    response = await service_client.post('v1/signup',
                                         json={'login': 'test@gmail.com',
                                               'password': 'qwerty',
                                               'full_name': 'arkady',
                                               'tg_id': '@test'})
    assert response.status == 200

    right_json = {'user_id': 1,
                  'login': 'test@gmail.com',
                  'full_name': 'arkady',
                  'tg_id': '@test'}

    assert response.json() == right_json

    user_id = response.json()['user_id']

    response = await service_client.get('v1/login', json={
        'login': 'test@gmail.com',
        'password': 'qwerty'})

    assert response.status == 200
    json = response.json()

    access_token = json['access_token']

    token = ' Bearer ' + access_token
    response = await service_client.get('v1/account',
                                        json={'user_id': user_id},
                                        headers={'Authorization': token})

    # Cache update interval is 10s =(
    while response.status == 403:
        response = await service_client.get('v1/account',
                                            json={'user_id': user_id},
                                            headers={'Authorization': token})

    assert response.status == 200
    assert response.json() == right_json


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_db_initial_data(service_client):
    response = await service_client.post(
        '/v1/hello',
        params={'name': 'user-from-initial_data.sql'},
    )
    assert response.status == 200
    assert response.text == 'Hi again, user-from-initial_data.sql!\n'
