#ifndef __EXTERNAL_IP_COUNTER_H__
#define __EXTERNAL_IP_COUNTER_H__

// A voting heat ends after the max number of votes have
// been counted or the heat duration (in seconds) expires,
// whichever comes last
#define EXTERNAL_IP_HEAT_DURATION	600	// 10 minutes
#define EXTERNAL_IP_HEAT_MAX_VOTES	50

#include <map>
#include "sockaddr.h"
#include "bloom_filter.h"

// allows the dht client to define what SHA-1 implementation to use
typedef sha1_hash SHACallback(byte const* buf, int len);

class ExternalIPCounter
{
public:
	ExternalIPCounter(SHACallback* sha);
	void CountIP( const SockAddr& addr, const SockAddr& voter );
	void CountIP( const SockAddr& addr );
	bool GetIP( SockAddr& addr ) const;
	bool GetIPv4( SockAddr& addr ) const;
	bool GetIPv6( SockAddr& addr ) const;

	void SetHeatStarted(time_t t) { _HeatStarted = t; }
private:
	void Rotate();
	bool IsExpired() const;

	typedef std::map<SockAddr, int> candidate_map;

	candidate_map _map;
	candidate_map::const_iterator _winnerV4;
	candidate_map::const_iterator _winnerV6;
	bloom_filter _voterFilter;
	time_t _HeatStarted;
	int _TotalVotes;

	SockAddr _last_winner4;
	SockAddr _last_winner6;
	int _last_votes4;
	int _last_votes6;

	SHACallback* _sha_callback;
};


#endif //__EXTERNAL_IP_COUNTER_H__