#ifdef CHANGED

// ne pas ajouter dans le Makefile!


template <class T, class U> SynchMap<T, U>::SynchMap()
{
	NewSem();
}

template <class T, class U> U& SynchMap<T, U>::Get(T key)
{
	return map[key];
}

template <class T, class U> void SynchMap<T, U>::Add(T key, U value)
{
	map[key] = value;
}

template <class T, class U> bool SynchMap<T, U>::TryGet(T key, U& value)
{
	typename std::map<T, U>::iterator it = map.find(key);
	if (it != map.end())
	{
		value = it->second;
		return true;
	}
	else
		return false;
}

template <class T, class U> void SynchMap<T, U>::Erase(T key)
{
	typename std::map<T, U>::iterator it = map.find(key);
	if (it != map.end())
		map.erase(it);
}

template <class T, class U> int SynchMap<T, U>::GetSize()
{
	return map.size();
}

template <class T, class U> void SynchMap<T, U>::SynchAdd(T key, U value)
{
	P();
	Add(key, value);
	V();
}

template <class T, class U> U& SynchMap<T, U>::SynchGet(T key)
{
	P();
	U& value = Get(key);
	V();
	return value;
}

template <class T, class U> bool SynchMap<T, U>::SynchTryGet(T key, U& value)
{
	P();
	bool found = TryGet(key, value);
	V();
	return found;
}

template <class T, class U> void SynchMap<T, U>::SynchErase(T key)
{
	P();
	Erase(key);
	V();
}

template <class T, class U> int SynchMap<T, U>::SynchGetSize()
{
	P();
	int size = GetSize();
	V();
	return size;
}

template <class T, class U> void SynchMap<T, U>::P()
{
	sem->P();
}

template <class T, class U> void SynchMap<T, U>::V()
{
	sem->V();
}

template <class T, class U> void SynchMap<T, U>::NewSem()
{
	sem = new Semaphore("SynchMap read", 1);
}

template <class T, class U> const std::map<T, U>& SynchMap<T, U>::GetMap()
{
	return map;
}

#endif


