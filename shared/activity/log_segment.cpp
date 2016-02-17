#include "activity/log_utility.hpp"
#include "activity/log_segment.hpp"

log_segment::log_segment(hash512 hash, container_type&& segment) {
    m_header.hash = hash;
    m_segment = std::move(segment);
    m_header.num_records = m_segment.size();
}

log_segment::log_segment(const log_segment& orig) {
    m_header = orig.m_header;
    m_segment = orig.m_segment;
}

log_segment::log_segment(const serial_ptr serial) {
	deserialise(serial);
}

log_segment::~log_segment() {
}

log_segment::serial_ptr log_segment::serialise() {
    auto hdr_ptr = reinterpret_cast<serial_ptr>(&m_header);
    auto hdr_sz = sizeof(log_header);

    auto heap = new serial_type[log_size()];
    auto ptr = heap;

    std::copy(hdr_ptr, hdr_ptr + hdr_sz, ptr);
    ptr += hdr_sz;

    for(auto& r : m_segment) {
            auto s = r.serialise();
            std::copy(s, s + r.size(), ptr);
            ptr += r.size();
            delete s;
    }

    return heap;
}

log_segment::size_type log_segment::log_size() {
    size_type sz = sizeof(log_header);
    for(auto& r : m_segment) {
            sz += r.size();
    }
    return sz;
}

log_segment::size_type log_segment::size() {
    return m_segment.size();
}

const log_header& log_segment::header() {
	return m_header;
}

log_segment::const_iterator log_segment::cbegin() {
	return m_segment.cbegin();
}

log_segment::const_iterator log_segment::begin() {
	return m_segment.cbegin();
}


log_segment::const_iterator log_segment::cend() {
	return m_segment.cend();
}

log_segment::const_iterator log_segment::end() {
	return m_segment.cend();
}


void log_segment::deserialise(const serial_ptr serial) {
	auto hdr = reinterpret_cast<log_header*>(serial);
	std::copy(hdr, hdr+1, &m_header);
	auto ptr = serial + sizeof(log_header);
	for(auto i = 0; i < m_header.num_records; i++) {
		activity a;
		a.deserialise(ptr);
		ptr += a.size();
		m_segment.push_back(a);
	}
}




