SELECT name FROM people WHERE people.id IN (SELECT DISTINCT person_id FROM stars, movies 
WHERE movies.id = stars.movie_id AND year = 2004) ORDER BY birth;